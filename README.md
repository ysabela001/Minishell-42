# 🐚 Minishell — 42 São Paulo

**Minishell** é um pequeno interpretador de comandos (shell) implementado em **C** como parte da formação da 42.  
Este repositório contém a implementação do shell, código-fonte em `src/`, uma versão local de `libft/` em `lib/libft` e os arquivos de build (Makefile).

---

## 📌 Sumário
- [Objetivo](#-objetivo-do-projeto)  
- [Estrutura do repositório](#-estrutura-do-repositório)  
- [Requisitos](#-requisitos)  
- [Como compilar](#-como-compilar)  
- [Como executar](#-como-executar)  
- [Builtins & exemplos de uso](#-builtins--exemplos-de-uso)  
- [Pipes, redirecionamentos e heredoc](#-pipes-redirecionamentos-e-heredoc)  
- [Tratamento de sinais](#-tratamento-de-sinais)  
- [Testes, valgrind e dicas de debug](#-testes-valgrind-e-dicas-de-debug)  
- [Checklist do avaliador (42)](#-checklist-do-avaliador-42)  
- [Próximos passos](#-próximos-passos)

---

## 🎯 Objetivo do projeto

Construir um shell POSIX-like que:

- execute comandos externos usando `fork` + `execve`,  
- trate pipes (`|`) e redirecionamentos (`>`, `<`, `>>`),  
- implemente builtins (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`),  
- suporte `here-doc` (`<<`),  
- gerencie sinais corretamente (SIGINT, EOF/Ctrl+D),  
- mantenha comportamento consistente frente a erros.

---

## 🗂️ Estrutura do repositório

```text
/
├── lib/
│   └── libft/         # biblioteca local (funções utilitárias)
├── src/               # código-fonte do minishell
├── Makefile           # regras de build
├── README.md          # (este arquivo)
└── readline.supp      # arquivo de suporte para testes de readline
```
Dentro de src/ estão os arquivos .c e .h do projeto (parser, executor, builtins, utils).
---

🛠️ Como compilar

Recomendado: usar o Makefile fornecido.

# na raiz do repositório
make

Comandos úteis do Makefile (padrão esperado):

make          # build
make clean    # remove .o
make fclean   # remove .o e binário
make re       # fclean + make
---

▶️ Como executar
./minishell
---
##Comandos##

Você verá um prompt personalizado (ex.: minishell$). Digite comandos como em um shell normal:

$ ls -la
$ echo Hello World
$ cd /tmp
$ pwd
$ export TEST=ok
$ echo $TEST
$ exit
---

🔧 Builtins — exemplos

echo [-n] [args...]

echo hello world
echo -n "no newline"


cd [dir]

cd /path/to/dir
cd  # volta ao HOME

pwd

export VAR=valor / unset VAR
export MYVAR=42
echo $MYVAR
unset MYVAR

env — mostra variáveis de ambiente

exit [status] — encerra o shell
---

⛓ Pipes, redirecionamentos e heredoc — exemplos

Pipe:

ls -la | grep minishell


Redirecionamento de saída:

echo "texto" > arquivo.txt


Append:

echo "mais" >> arquivo.txt


Redirecionamento de entrada:

wc -l < arquivo.txt


Here-doc:

cat << EOF
linha1
linha2
EOF
---

🔔 Tratamento de sinais

Ctrl+C (SIGINT): interrompe processo em foreground, mas não deve terminar o shell. O prompt deve reaparecer.

Ctrl+D: quando digitado em prompt vazio, deve encerrar o shell (exit). Em outros contextos, enviado como EOF para o programa em foreground.

Teste interações de sinais com processos em foreground e com builtins.
---

🧪 Testes, Valgrind & Debug

Valgrind (Linux/WSL/macOS via brew):

valgrind --leak-check=full --show-leak-kinds=all ./minishell
---

Debug básico:

printf dentro do código para entender fluxo

gdb para depuração passo-a-passo
---

Testes automatizados:

Pode-se usar readline.supp (já presente no repo) para rodar casos de teste automatizados com o binário. Exemplo:

bash readline.supp ./minishell
