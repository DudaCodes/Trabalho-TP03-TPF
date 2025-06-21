
# 🧮 Avaliador de Expressões Numéricas – TP03 (ED/UCB)

Trabalho Prático 03 da disciplina de **Estrutura de Dados** do curso de Ciência da Computação da **Universidade Católica de Brasília – 2025/1**.

Este projeto implementa um avaliador de expressões matemáticas que:
- Converte expressões **infixas** para **pós-fixadas**
- Avalia expressões **pós-fixadas**
- Avalia expressões **infixas diretamente**
- Suporta **operadores matemáticos e funções trigonométricas**

---

## ✅ Funcionalidades

- [x] Conversão infixa → pós-fixada (`getFormaPosFixa`)
- [x] Avaliação pós-fixada (`getValorPosFixa`)
- [x] Avaliação direta da infixada (`getValorInFixa`)
- [x] Suporte aos operadores: `+ - * / % ^`
- [x] Suporte às funções: `sen`, `cos`, `tg`, `log`, `raiz`
- [x] Menu interativo no terminal
- [x] Testes automáticos com expressões do enunciado

---

## 🗂️ Estrutura dos Arquivos

| Arquivo                 | Descrição                                       |
|--------------------------|------------------------------------------------|
| `expressao.c`            | Implementação da lógica                        |
| `main.c`                 | Interface de testes e menu                     |
| `expressao.h`            | Header com definições de funções e struct      |
| `expressao.exe`          | Arquivo executável compilado (Windows)         |

---

## ⚙️ Como compilar

No terminal, dentro da pasta do projeto:

```bash
gcc expressao.c main.c -o expressao -lm
```

> O `-lm` é necessário para usar a biblioteca `math.h`

Para rodar:

```bash
./expressao       # Linux/macOS
expressao.exe     # Windows
```

---

## 👥 Integrantes do Grupo

- **Ana Beatriz Alves** 
- **David Cordeiro Moura**
- **Eduarda Alves**
---

## 🧠 Professor

**Marcelo Eustáquio**  
Disciplina: Estrutura de Dados – 3º semestre de 2025  
Universidade Católica de Brasília – UCB

---
