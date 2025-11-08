// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/bytecode/data_section.h"

#include <functional>

#include "BibbleVM/core/vm.h"

namespace bibble {
    std::optional<CallEntry> CallEntry::ReadFromSection(const Section& section, size_t offset) {
        std::optional<u32> module = section.getU32(offset);
        std::optional<u32> address = section.getU32(offset + 4);
        if (!module.has_value() || !address.has_value()) return std::nullopt;

        std::optional<std::array<u8, 8>> name;
        if (address.value() == 0xFFFFFFFF) {
            name.emplace();
            if (!section.getBytes<8>(offset + 8, name->data())) return std::nullopt;
        }

        return CallEntry(module.value(), address.value(), name);
    }

    bool CallEntry::writeToSection(Section& section, size_t offset) const {
        if (!section.setU32(offset, module)) return false;
        if (!section.setU32(offset + 4, address)) return false;

        if (name.has_value()) {
            if (!section.setBytes<8>(offset + 8, name.value().data())) return false;
        }

        return true;
    }

    DataSection::DataSection(Section section)
        : mSection(section) {}

    std::optional<i8> DataSection::getByte(u32 offset) {
        return mSection.getI8(offset);
    }

    std::optional<i16> DataSection::getShort(u32 offset) {
        return mSection.getI16(offset);
    }

    std::optional<i32> DataSection::getInt(u32 offset) {
        return mSection.getI32(offset);
    }

    std::optional<i64> DataSection::getLong(u32 offset) {
        return mSection.getI64(offset);
    }

    std::optional<float> DataSection::getFloat(u32 offset) {
        return mSection.getFloat(offset);
    }

    std::optional<double> DataSection::getDouble(u32 offset) {
        return mSection.getDouble(offset);
    }

    std::optional<std::string_view> DataSection::getString(u32 offset, const StrtabSection& strtab) {
        std::optional<const u8*> bytesOpt = mSection.getBytes<8>(offset);
        if (!bytesOpt.has_value()) return std::nullopt;

        const u8* bytes = bytesOpt.value();

        return resolveString(bytes, strtab);
    }

    const CallableTarget* DataSection::getCallable(u32 offset, VM& vm) {
        auto cached = mCallableCache.find(offset);
        if (cached != mCallableCache.end()) return cached->second;

        std::optional<CallEntry> callEntryOpt = mSection.getCustom<CallEntry>(offset);
        if (!callEntryOpt.has_value()) return nullptr;

        CallEntry& callEntry = callEntryOpt.value();

        if (callEntry.module == 0xFFFFFFFF) { // module not linked yet
            Module* currentModule = vm.currentModule();
            if (currentModule == nullptr) return nullptr;

            if (callEntry.address == 0xFFFFFFFF) { // function not resolved
                if (!callEntry.name.has_value()) return nullptr;

                std::optional<std::string_view> name = resolveString(callEntry.name.value().data(), currentModule->strtab());
                if (!name.has_value()) return nullptr;

                Function* function = vm.getFunction(name.value());
                if (function == nullptr) return nullptr;

                callEntry.module = function->getModule();
                callEntry.address = static_cast<u32>(function->target().entry.getPosition()); // TODO: make this more CallableTarget-generic (e.g. machine code)

                mSection.setCustom<CallEntry>(offset, callEntry);

                CallableTarget* target = &function->target();
                mCallableCache[offset] = target;
                return target;
            } else { // function is resolved and is in current module
                std::optional<BytecodeReader> bytecode = currentModule->code().getBytecodeReader(callEntry.address);
                if (!bytecode.has_value()) return nullptr;

                callEntry.module = vm.currentModuleH();

                mCallableTargets.push_back(std::make_unique<CallableTarget>(callEntry.module, bytecode.value()));

                mSection.setCustom<CallEntry>(offset, callEntry);

                CallableTarget* target = mCallableTargets.back().get();
                mCallableCache[offset] = target;
                return target;
            }
        } else { // this is already linked, but somehow not cached?
            return nullptr;
        }
    }

    std::optional<std::string_view> DataSection::resolveString(const u8 bytes[8], const StrtabSection& strtab) {
        if (bytes[0] == '@' && bytes[1] == 'S' && bytes[2] == 'T' && bytes[3] == 'R') {
            u32 stringOffset = (static_cast<u32>(bytes[4]) << 24) |
                               (static_cast<u32>(bytes[5]) << 16) |
                               (static_cast<u32>(bytes[6]) << 8) |
                               (static_cast<u32>(bytes[7]));

            return strtab.get(stringOffset);
        } else {
            size_t len = 0;
            for (int i = 0; i < 8; i++) {
                if (bytes[i] == 0) break;
                len++;
            }

            return std::string_view(reinterpret_cast<const char*>(bytes), len);
        }
    }
}
