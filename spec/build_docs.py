import yaml, os
from jinja2 import Environment, FileSystemLoader

env = Environment(loader=FileSystemLoader('templates'))
tpl = env.get_template('instructions.md.j2')

with open('data/instructions.yaml') as f:
    instructions = yaml.safe_load(f)

out_path = f"docs/instructions.md"
with open(out_path, 'w') as out:
    out.write(tpl.render(instructions=instructions))
