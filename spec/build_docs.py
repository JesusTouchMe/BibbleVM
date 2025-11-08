import yaml, os
from jinja2 import Environment, FileSystemLoader

env = Environment(loader=FileSystemLoader('templates'))
tpl = env.get_template('instruction_page.md.j2')

os.makedirs('docs/instructions', exist_ok=True)

with open('data/instructions.yaml') as f:
    instructions = yaml.safe_load(f)

for ins in instructions:
    out_path = f"docs/instructions/{ins['name'].lower()}.md"
    with open(out_path, 'w') as out:
        out.write(tpl.render(**ins))
