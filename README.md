# TPA: Processamento do Tupi Antigo

O presente trabalho tem como objetivo preservar o Tupi Antigo, língua falada tanto pelos povos nativos quanto pelos colonizadores nos primeiros séculos da colonização. Para isso, são utilizados de forma justa e ética os estudos do professor Eduardo de Almeida Navarro como base de dados e treinamento para os modelos desenvolvidos neste projeto. O foco é estritamente educacional, buscando contribuir para a preservação da memória e cultura dos povos indígenas que habitaram o Brasil Colônia.

É usado o GPT2 small  para treinamento.
Inicialmente é feito um treinamento para gerar o modelo base, 
em seguida é realizado o ajuste fino para que o modelo melhore 
no processamneto das entradas em tupi antigo e suas respectivas
traduções em português.

O projeto é divididio em diversos módulos:
- app
    - dados
        - dicionário
            - arquivos de dicionário
        - modelo
            - arquivos do modelo
        - script para criar o dataset
        - script para contar tokens
        - dataset
        - fontes bibliográficas
    - código fonte
        - módulo principal
        - módulo de conversa
        - módulo de dicionário
        - módulo de interface
        - módulo de primeiro treino
        - módulo de ajuste fino

O programa principal é escrito em C com interface gráfica em Gtk4, o módulo de dicionário é feito totalmente em C, já o módulo de conversa é feito em Python e usa a bibliotecas Transformers, Dataset e Torch para treinamento do modelo.

O modelo quando treinado com um dataset satisfatório deve ser capaz de:
    - Tradução entre dois idiomas.
    - Geração de texto em diferentes contextos.
    - Geração de respostas em formato de chat.

A aplicação ainda está em fase de implementação e coleta de dados. Os dados são digitalizados do Sicionário de Tupi Antigo e do Método Moderno de Tupi Antigo do Professor Eduardo de Almeida Navarro.

### Técnico

Configurações do modelo:
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

Treinamento:
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
   
Parâmetros:
    vocab_size: 50,257
    n_embd: 512
    n_layer: 6
    n_head: 8
    intermediate_size: 2048 (4 × n_embd)

    Parâmetros de Embedding:
    50,257×512=25,742,144
    50,257×512=25,742,144

    Parâmetros de Atenção (por camada): Para uma camada de atenção:
    4×(512×512)+2×(512×8)=1,048,576+8,192=1,056,768
    4×(512×512)+2×(512×8)=1,048,576+8,192=1,056,768

    Para 6 camadas de atenção:
    1,056,768×6=6,340,608
    1,056,768×6=6,340,608

    Parâmetros Feed-Forward (por camada): Para cada camada feed-forward:
    2×(512×2048)=2,097,152
    2×(512×2048)=2,097,152

    Para 6 camadas feed-forward:
    2,097,152×6=12,582,912
    2,097,152×6=12,582,912

    Parâmetros de Normalização e Viés (aproximadamente para 6 camadas): Esses parâmetros são pequenos em comparação, então podemos estimar em torno de 1,000 parâmetros por camada (devido ao viés e normalização), resultando em um total de aproximadamente 6,000 parâmetros.

Somando tudo:

    Parâmetros de Embedding: 25,742,144
    Parâmetros de Atenção: 6,340,608
    Parâmetros Feed-Forward: 12,582,912
    Parâmetros de Normalização e Viés: 6,000

Total de parâmetros aproximado:
    25,742,144+6,340,608+12,582,912+6,000≈44,671,664
    25,742,144+6,340,608+12,582,912+6,000≈44,671,664

Total de parâmetros: Aproximadamente 44.7 milhões de parâmetros.