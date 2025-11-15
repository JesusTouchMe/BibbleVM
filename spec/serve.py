import mkdocs
import subprocess
import sys

import build_docs

subprocess.run([sys.executable, "-m", "mkdocs", "serve"])
