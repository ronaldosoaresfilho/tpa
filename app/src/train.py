from transformers import GPT2Config, GPT2LMHeadModel, AutoTokenizer, Trainer, TrainingArguments, DataCollatorForLanguageModeling, AutoModelForCausalLM
from datasets import Dataset
import json
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# caminho para o arquivo da memória
MEMORY_FILE = os.path.join(BASE_DIR, "../data/chat_memory.json")

# caminho do modelo
MODEL_DIR = os.path.join(BASE_DIR, "../data/model")
OUTPUT_DIR = MODEL_DIR

def clear_memory():
    with open(MEMORY_FILE, "w", encoding="utf-8") as f:
        f.write("[]")

def load_memory():
    with open(MEMORY_FILE, "r", encoding="utf-8") as f:
        return json.load(f)

def prepare_training_data_from_json(memory):
    texts = []

    for interaction in memory:
        if interaction["role"] == "user":
            texts.append(f"User: {interaction['content']}\n")
        elif interaction["role"] == "chat":
            texts[-1] += f"Chat: {interaction['content']}\n\n"
    return Dataset.from_dict({"text": texts})


def train_fine_tune():

    # carrega o modelo
    model = AutoModelForCausalLM.from_pretrained(MODEL_DIR, local_files_only=True, from_tf=False)
    tokenizer = AutoTokenizer.from_pretrained(MODEL_DIR, local_files_only=True)

    # Adiciona os tokens
    tokenizer.add_tokens(["->"])
	tokenizer.add_tokens(['"'])
	tokenizer.add_tokens(["..."])
	tokenizer.add_tokens(["."])
	tokenizer.add_tokens([","])
	tokenizer.add_tokens([" "])
	tokenizer.add_tokens(["#"])
	tokenizer.add_tokens([" "])
	tokenizer.add_tokens(["("])
	tokenizer.add_tokens([")"])
	tokenizer.add_tokens(["\n"])

    # Ajusta o modelo para os novos tokens
    model.resize_token_embeddings(len(tokenizer))

    # carregar e preparar os dados
    memory = load_memory()
    dataset = prepare_training_data_from_json(memory)

    # tokenizar o dataset
    def tokenize_function(examples):
        return tokenizer(examples["text"], truncation=True, padding=True, max_length=512)

    tokenized_dataset = dataset.map(tokenize_function, batched=True)
    
    # configurar data collator
    data_collator = DataCollatorForLanguageModeling(tokenizer=tokenizer, mlm=False)

    # configurar treinamento
    training_args = TrainingArguments(
            output_dir=OUTPUT_DIR,
            overwrite_output_dir=False, # não sobrescreve o modelo anterior
            num_train_epochs=10,
            per_device_train_batch_size=8,
            save_steps=0,
            save_strategy="no",
            save_total_limit=2,
            logging_dir="../data/logs_fine_tune",
            logging_steps=10,
            learning_rate=5e-4,
            weight_decay=0.01,
            warmup_steps=500,
            eval_strategy="no",
            )

    # configurar o trainer
    trainer = Trainer(
            model=model,
            args=training_args,
            data_collator=data_collator,
            train_dataset=tokenized_dataset,
            )

    # treinar o modelo
    trainer.train()

    # salvar o modelo treinado
    model.save_pretrained(OUTPUT_DIR, safe_serialization=True)
    tokenizer.save_pretrained(OUTPUT_DIR)

def train_end():
    with open("train_end.txt", "w", encoding="utf-8") as f:
        f.write("ok")    

if __name__ == "__main__":
    train_fine_tune()
    clear_memory()
    train_end()
