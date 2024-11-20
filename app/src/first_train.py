from transformers import GPT2Config, GPT2LMHeadModel, AutoTokenizer, Trainer, TrainingArguments, DataCollatorForLanguageModeling
from datasets import Dataset

# Caminho para os arquivos
DATASET_FILE = "../data/dataset.txt"
OUTPUT_DIR = "../data/model"

def prepare_training_data_from_txt(file_path):
    with open(file_path, "r", encoding="utf-8") as f:
        text = f.read()
    return Dataset.from_dict({"text": text.split("\n\n")}) # divide pelos diálogos

def first_train():
    # configuração do modelo
    config = GPT2Config(
            vocab_size=50257,               # tamanho do vocabulário
            n_positions=512,                # tamanho máximo de sequência
            n_embd=512,                     # tamanho do embedding
            n_layer=6,                      # número de camadas
            n_head=8,                       # número de cabeças de atenção
            activation_function="gelu_new", # função de ativação
            initializer_range=0.02,         # inicialização dos pesos
            layer_norm_epsilon=1e-5,        # epsilon para normalização       
            bos_token_id=0,                 # id do token de início da sequência
            eos_token_id=1,                 # id do token do fim da sequência
            use_cache=True,                 # habilitar o cache
            torch_dtype="float32",           # tipo de dado para o modelo
            )

    # inicalizador do modelo do zero
    model = GPT2LMHeadModel(config)

    # tokenizador
    tokenizer = AutoTokenizer.from_pretrained("gpt2")

    # define o token de padding
    tokenizer.pad_token = tokenizer.eos_token

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
        output_dir=OUTPUT_DIR,             # Diretório de saída para o modelo final
        overwrite_output_dir=True,         # Sobrescrever o diretório de saída
        num_train_epochs=10,               # Número de épocas para treinamento
        per_device_train_batch_size=8,     # Tamanho do lote
        save_steps=0,                      # Não salvar checkpoints durante o treinamento
        save_strategy="no",                # Não salvar checkpoints em nenhuma estratégia
        save_total_limit=2,                # Limitar número total de checkpoints (opcional)
        logging_dir="../data/logs",        # Diretório para logs
        logging_steps=10,                  # Passos de logging
        learning_rate=5e-4,                # Taxa de aprendizado
        weight_decay=0.01,                 # Decaimento de peso
        warmup_steps=500,                  # Passos de aquecimento
        eval_strategy="no",                # Não realizar avaliação durante o treinamento
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

if __name__ == "__main__":
    first_train()
