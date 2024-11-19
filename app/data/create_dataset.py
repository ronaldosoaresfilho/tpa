# script que faz parse no dicionário e gera o dataset.txt

import os
import json

# Caminho para o diretório dos arquivos JSON e para o arquivo de saída
INPUT_DIR = "dictionary"
OUTPUT_FILE = "dataset.txt"

def sanitize(text):
    """
    Remove reticências no início e no fim de uma string
    e espaços em branco ao redor.
    """
    # Remove espaços no início e fim
    text = text.strip()
    # Remove reticências no início
    text = text.lstrip('.').strip()
    # Remove reticências no fim
    text = text.rstrip('.').strip()
    return text

def extract_data_from_json(file_path):
    """
    Extrai dados do arquivo JSON no formato esperado.
    """
    with open(file_path, 'r', encoding='utf-8') as f:
        data = json.load(f)

    entries = []
    # Iterar sobre as palavras no dicionário
    for letter, content in data.get("root", {}).items():
        for entry in content.get("entries", []):
            word = entry.get("word", "").strip()
            meanings = entry.get("meaning", [])

            # Adicionar a palavra e sua tradução básica
            for meaning in meanings:
                for case in meaning.get("cases", []):
                    translation = case.get("pt", "").strip()
                    if word and translation:
                        word = sanitize(word)
                        translation = sanitize(translation)
                        entries.append(f"{word} -> {translation}")
                        
                    # Adicionar os exemplos
                    for example in case.get("examples", []):
                        tp = example.get("tp", "")
                        pt = example.get("pt", "")
                        tp = sanitize(tp)
                        pt = sanitize(pt)
                        if tp and pt:
                            entries.append(f"{tp} -> {pt}")
                        
    return entries

def create_dataset():
    """
    Cria um dataset concatenando dados de todos os arquivos JSON.
    """
    dataset = []
    for filename in os.listdir(INPUT_DIR):
        if filename.endswith(".json"):
            file_path = os.path.join(INPUT_DIR, filename)
            dataset.extend(extract_data_from_json(file_path))

    # Escrever no arquivo de saída
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        for line in dataset:
            f.write(line + "\n")

    print(f"Dataset criado com sucesso! Salvo em: {OUTPUT_FILE}")

if __name__ == "__main__":
    create_dataset()
