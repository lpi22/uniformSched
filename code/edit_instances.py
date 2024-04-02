import os
import random

path = 'instances_identical\\PSMF\\BIG' 
new_path = 'instances_uniform\\BIG' 



for file in os.listdir(path):
    if file.endswith('.dat'):
        file_pfad = os.path.join(path, file)

        with open(file_pfad, 'r') as datei:
            lines = datei.readlines()

        new_file_path = os.path.join(new_path, file)

        used_processing_times = []

        with open(new_file_path, 'w') as new_file:
            for line_number, value in enumerate(lines):
                if line_number == 0:
                    num_machines = int(value.strip())
                    first_line = f'{num_machines}\n'
                    new_file.write(first_line)
                    s = 1
                    while num_machines > 0:
                        rand_num = random.randint(1, num_machines)
                        new_line = f'{rand_num},{s}\n'
                        num_machines -= rand_num
                        s += 1
                        new_file.write(new_line)
                elif line_number == 1:
                    num_jobs = int(value.strip())
                    next_line = f'{num_jobs}\n'
                    new_file.write(next_line)
                elif num_jobs > 0:
                    if not value in used_processing_times:
                        rand_num = random.randint(1, num_jobs)
                        new_line = f'{rand_num},{value.strip()}\n'
                        num_jobs -= rand_num
                        new_file.write(new_line)
                        used_processing_times.append(value)

        print(f'File {file} has been edited and saved as {new_file_path}.')

print('Done.')
