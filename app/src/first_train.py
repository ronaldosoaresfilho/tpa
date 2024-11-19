from transformers import GPT2Config, GPT2LMHeadModel, AutoTokenizer, Trainer, TrainingArguments, DataCollatorForLanguageModeling
from datasets import Dataset

# Caminho para os arquivos
DATASER_FILE = "../../app/data/daset.txt"
OUTPUT_DIR = "../../app/data/model"

def prepare_training_data_from_txt(file_path):

