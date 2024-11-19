from transformers import GPT2Config, GPT2LMHeadModel, AutoTokenizer, Trainer, TrainingArguments, DataCollatorForLanguageModeling
from datasets import Dataset

MEMORY_FILE = '../../app/data/chat_memory.py'
MODEL_DIR = "../../app/data/model"
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
    model = AutoModelForCausalLM.from_pretrained(MODEL_DIR)
    tokenizer = AutoTokenizer.from_pretrained(MODEL_DIR)

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
            overwrite_output_dir=True,
            num_train_epochs=5,
            per_device_train_batch_size=2,
            save_steps=500,
            save_total_limit=2,
            logging_dir="../../app/data/logs_fine_tune",
            logging_steps=10,
            learning_rate=5e-4,
            weight_decay=0.01,
            warmup_steps=500,
            evaluation_strategy="no",
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
    model.save_pretrained(OUTPUT_DIR)
    tokenizer.save_pretrained(OUTPUT_DIR)

if __name__ == "__main__":
    train_fine_tune()
    clear_memory()
