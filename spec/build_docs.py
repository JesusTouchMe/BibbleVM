import yaml, os
from jinja2 import Environment, FileSystemLoader

env = Environment(loader=FileSystemLoader('templates'))
tpl = env.get_template('instructions.md.j2')

with open('data/instructions.yaml', encoding='utf-8') as f:
    instructions = yaml.safe_load(f)

out_path = "docs/instructions.md"
with open(out_path, 'w', encoding='utf-8') as out:
    out.write(tpl.render(instructions=instructions))
