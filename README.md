# TPA: Processamento do Tupi Antigo

O presente trabalho tem como objetivo preservar o Tupi Antigo, língua falada tanto pelos povos nativos quanto pelos colonizadores nos primeiros séculos da colonização. Para isso, são utilizados de forma justa e ética os estudos do professor Eduardo de Almeida Navarro como base de dados e treinamento para os modelos desenvolvidos neste projeto. O foco é estritamente educacional, buscando contribuir para a preservação da memória e cultura dos povos indígenas que habitaram o Brasil Colônia.

É usado o GPT2 para treinamento.
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
        - dataset
        - fontes bibliográficas
    - código fonte
        - módulo principal
        - módulo de conversa
        - módulo de dicionário
        - módulo de interface
        - módulo de primeiro treino
        - módulo de ajuste fino

O programa é principal é escrito em C com interface gráfica em Gtk4, o módulo de dicionário é feito totalmente em C, já o módulo de conversa é feito em Python e usa a bibliotecas Transformers, Dataset e Torch para treinamento do modelo.

A aplicação ainda está em fase de implementação e coleta de dados. Os dados são digitalizados do Sicionário de Tupi Antigo e do Método Moderno de Tupi Antigo do Professor Eduardo de Almeida Navarro.