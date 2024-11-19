MEMORY_FILE = '../../app/data/chat_memory.py'
DATASET = '../../app/data/dataset.txt'

def clear_memory():
    with open(MEMORY_FILE, "w", encoding="utf-8") as f:
        f.write("[]")

if __name__ == "__main__":
    clear_memory()
