from transformers import GPT2Config, GPT2LMHeadModel, AutoTokenizer, Trainer, TrainingArguments, DataCollatorForLanguageModeling
from datasets import Dataset

# Caminho para os arquivos
DATASER_FILE = "../../app/data/daset.txt"
OUTPUT_DIR = "../../app/data/model"

def prepare_training_data_from_txt(file_path):
    with open(file_path, "r", encoding="utf-8") as f:
        text = f.read()
    return Dataset.from_dict({"text": text.split("\n\n")}) # divide pelos diálogos

def first_train():
    # configuração do modelo
    config = GPT2Config(
            vocab_size=50257,
            n_positions=1024,
            n_ctx=1024,
            n_embd=768,
            n_layer=12,
            n_head=12,
            bos_token_id=0,
            eos_token_id=1,
            )

    # inicalizador do modelo do zero
    model = GPT2LMHeadModel(config)

    # tokenizador
    tokenizer = AutoTokenizer.from_pretrained("gpt2")

    # carregar e preparar dados
    dataset = prepare_training_data_from_txt(DATASET_FILE)

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
            logging_dir="../../app/data/logs",
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
    first_train()
