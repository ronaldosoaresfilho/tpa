# chat.py usando GTP2
from transformers import GPT2LMHeadModel, GPT2Tokenizer
import torch
import json
import sys
import os

# caminho para o arquivo da memória
MEMORY_FILE = "../../app/data/chat_memory.json"

# caminho do modelo
MODEL_PATH = "model"
model = GPT2LMHeadModel.from_pretrained(MODEL_PATH)
tokenizer = GPT2Tokenizer.from_pretrained(MODEL_PATH)

def save_memory(memory):
    with open(MEMORY_FILE, "w", encoding="utf-8") as f:
        json.dump(memory, f, ensure_ascii=False, indent=4)


def generate_response(input_text):
    # codifica o texto de entrada
    inputs = tokenizer.encode(input_text, return_tensors="pt")

    # gerar a resposta do modelo
    outputs = model.generate(inputs, max_length=150, num_return_sequences=1, no_repeat_ngram_size=2, pad_token_id=tokenizer.eos_token_id)

    # decodificar a resposta gerada
    response = tokenizer.decode(outputs[0], skip_special_tokens=True)
    
    return response

def chatbot():
    # pega a entrada do código C
    with open('input.txt', 'r') as f:
        user_input = f.read().strip()

    # adicionar na memória
    memory.append({"role": "user", "content": user_input})

    # gerar a resposta do modelo
    response = generate_response(user_input)

    # retorna a resposta para o código C
    with open('output.txt', 'w') as f:
        f.write(response);

    # salvar histórico de memória
    memory.append({"role": "chatbot", "content": response})

if __name__ == "__main__":
    chatbot()
