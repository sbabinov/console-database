import random

last_names = [
    "Adamson",
    "Becker",
    "Birch",
    "Bishop",
    "Boolman",
    "Chapman",
    "Clifford",
    "Cook",
    "Daniels",
    "Day",
    "Derrick",
    "Goldman",
    "Hawkins",
    "Higgins",
    "Kennedy",
    "Lawman",
    "Larkins",
    "Leman",
    "Lewin",
    "Little",
    "Otis",
    "Sherlock",
    "Samuels"
]

with open("hash_table_test_data.txt", "w") as f:
    for i in range(2_000_000):
        data = f"{i} {random.choice(last_names)}\n"
        f.write(data)
