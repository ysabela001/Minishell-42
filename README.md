# 🐚 Minishell — 42 São Paulo

**Minishell** é um interpretador de comandos (shell) implementado em **C** como parte do currículo da 42 São Paulo. O projeto replica funcionalidades básicas de um shell POSIX, incluindo execução de comandos, pipes, redirecionamentos e builtins.

---

## 📌 Sumário

- [Objetivo do Projeto](#-objetivo-do-projeto)
- [Estrutura do Repositório](#️-estrutura-do-repositório)
- [Compilação](#️-compilação)
- [Execução](#️-execução)
- [Funcionalidades](#-funcionalidades)
  - [Builtins](#builtins)
  - [Pipes e Redirecionamentos](#pipes-e-redirecionamentos)
  - [Tratamento de Sinais](#tratamento-de-sinais)
- [Testes](#-testes)
- [Checklist de Avaliação](#-checklist-de-avaliação)
- [Próximos Passos](#-próximos-passos)

---

## 🎯 Objetivo do Projeto

Desenvolver um shell minimalista POSIX-like com as seguintes características:

- Execução de comandos externos via `fork` + `execve`
- Implementação de pipes (`|`) e redirecionamentos (`>`, `<`, `>>`)
- Suporte a here-doc (`<<`)
- Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Gerenciamento correto de sinais (SIGINT, EOF)
- Expansão de variáveis de ambiente
- Tratamento robusto de erros

---

## 🗂️ Estrutura do Repositório

```
.
├── lib/
│   └── libft/          # Biblioteca de funções auxiliares
├── src/                # Código-fonte do minishell
│   ├── parser/         # Análise léxica e sintática
│   ├── executor/       # Execução de comandos
│   ├── builtins/       # Implementação dos builtins
│   └── utils/          # Funções utilitárias
├── Makefile            # Regras de compilação
├── README.md           # Documentação do projeto
└── readline.supp       # Supressões para testes com Valgrind
```

---

## ⚙️ Compilação

Na raiz do repositório, execute:

```bash
make
```

### Comandos disponíveis

| Comando       | Descrição                                    |
|---------------|----------------------------------------------|
| `make`        | Compila o projeto                            |
| `make clean`  | Remove arquivos objeto (`.o`)                |
| `make fclean` | Remove arquivos objeto e o executável        |
| `make re`     | Recompila o projeto do zero (`fclean + make`)|

---

## ▶️ Execução

Após a compilação, execute o shell:

```bash
./minishell
```

Você verá um prompt interativo onde poderá digitar comandos:

```bash
minishell$ ls -la
minishell$ echo "Hello, World!"
minishell$ exit
```

---

## 🔧 Funcionalidades

### Builtins

O Minishell implementa os seguintes comandos internos:

#### `echo`
Imprime argumentos na saída padrão.

```bash
minishell$ echo Hello World
Hello World

minishell$ echo -n "sem quebra de linha"
sem quebra de linha$
```

#### `cd`
Altera o diretório de trabalho atual.

```bash
minishell$ cd /tmp
minishell$ cd          # retorna para $HOME
minishell$ cd ..       # sobe um diretório
```

#### `pwd`
Exibe o diretório de trabalho atual.

```bash
minishell$ pwd
/home/user/minishell
```

#### `export`
Define ou exibe variáveis de ambiente.

```bash
minishell$ export VAR=valor
minishell$ echo $VAR
valor
```

#### `unset`
Remove variáveis de ambiente.

```bash
minishell$ unset VAR
```

#### `env`
Lista todas as variáveis de ambiente.

```bash
minishell$ env
```

#### `exit`
Encerra o shell com status opcional.

```bash
minishell$ exit
minishell$ exit 42
```

---

### Pipes e Redirecionamentos

#### Pipe (`|`)
Encadeia a saída de um comando na entrada de outro.

```bash
minishell$ ls -la | grep minishell
minishell$ cat file.txt | wc -l
```

#### Redirecionamento de Saída (`>`, `>>`)

```bash
minishell$ echo "texto" > arquivo.txt      # sobrescreve
minishell$ echo "mais texto" >> arquivo.txt # append
```

#### Redirecionamento de Entrada (`<`)

```bash
minishell$ wc -l < arquivo.txt
```

#### Here-doc (`<<`)
Lê entrada até encontrar um delimitador.

```bash
minishell$ cat << EOF
> linha 1
> linha 2
> EOF
linha 1
linha 2
```

---

### Tratamento de Sinais

| Sinal          | Comportamento                                                |
|----------------|--------------------------------------------------------------|
| **Ctrl+C**     | Interrompe processo em foreground, exibe novo prompt         |
| **Ctrl+D**     | Envia EOF; em prompt vazio, encerra o shell                  |
| **Ctrl+\\**    | Ignorado (não deve terminar o shell)                         |

---

## 🧪 Testes

### Valgrind

Para verificar vazamentos de memória:

```bash
valgrind --leak-check=full --show-leak-kinds=all \
         --suppressions=readline.supp ./minishell
```


### Testes Automatizados

Utilize o script de testes fornecido:

```bash
bash readline.supp ./minishell
```

### Casos de Teste Sugeridos

- Comandos simples: `ls`, `cat`, `echo`
- Pipes múltiplos: `ls | grep c | wc -l`
- Redirecionamentos combinados: `< in.txt cat | grep word > out.txt`
- Variáveis: `export X=1; echo $X$X`
- Aspas: `echo "teste com $VAR"` vs `echo 'teste com $VAR'`
- Sinais: testar Ctrl+C durante execução de comandos

---

## ✅ Checklist de Avaliação

### Compilação
- [ ] `make` compila sem erros
- [ ] `make clean` remove objetos
- [ ] `make fclean` remove tudo
- [ ] `make re` recompila corretamente

### Funcionalidades Básicas
- [ ] Prompt é exibido corretamente
- [ ] Comandos simples funcionam (`ls`, `cat`, etc.)
- [ ] Histórico de comandos funciona (↑/↓)

### Builtins
- [ ] `echo` com e sem `-n`
- [ ] `cd` com caminhos relativos e absolutos
- [ ] `pwd` exibe caminho correto
- [ ] `export` e `unset` gerenciam variáveis
- [ ] `env` lista variáveis
- [ ] `exit` com e sem código de status

### Pipes e Redirecionamentos
- [ ] Pipes simples e múltiplos
- [ ] `>`, `>>`, `<` funcionam corretamente
- [ ] Here-doc (`<<`) funciona

### Sinais
- [ ] Ctrl+C não termina o shell
- [ ] Ctrl+D encerra corretamente
- [ ] Sinais em processos filho são tratados

### Qualidade do Código
- [ ] Sem memory leaks (Valgrind)
- [ ] Código segue norminette (se aplicável)
- [ ] Tratamento adequado de erros

---

## 👥 Autores

Desenvolvido como parte do currículo da **42 São Paulo** pela Ysabela Tavares.

---

## 📄 Licença

Este projeto é desenvolvido para fins educacionais na 42 São Paulo.
