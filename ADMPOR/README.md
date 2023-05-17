# Projeto - Fase Final

Trabalho desenvolvido para o disciplina de Sistemas Operativos da Faculdade de Ciências da Universidade de Lisboa

## Uso do programa
O programa ADMPOR é composto por 4 diretórios:

 - /bin - Onde se encontrará o executável com o nome **admpor** , depois do programa ser compilado.
 - /include - Ficheiro **.h**  com a definição das estruturas de dados e declarações de funções usadas.
 - /obj - Aqui se entrarão os ficheiros objeto **.o** depois da compilação do programa.
 - /src - Ficheiros fonte **.c** com o código das funções desenvolvidas.

Além destes 4 diretórios, é fornecido um ficheiro de configuração **config.txt** para ser utilizado como input ao executar o programa.

É também fornecido um ficheiro *Makefile* para a compilação do programa através do comando **make** na linha de comandos. Pode ser ainda usado o comando **make clean** para serem automaticamente apagados os ficheiros objecto **.o** e o executável **admpor** gerados pela compilação. Este mesmo comando também apaga os ficheiros **log.txt** e **stats.txt** gerados durante a execução do programa.

Para executar o programa, a seguir à compilação do mesmo, basta executar o ficheiro **admpor** gerado que se encontrará na pasta *bin*, seguido do ficheiro de configuração. Exemplo: **$ ./bin/admpor config.txt**

### Trabalho realizado por:

**Grupo 10** <br />
Eduardo Proença - 57551 <br />
Tiago Oliveira - 54979 <br />

#### Todas as funcionalidades pedidas foram implementadas no projeto.
