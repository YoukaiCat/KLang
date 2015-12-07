# The Koryaga Programming Language
Koryaga is a new single-typed expression-centred general-purpose programming language.  
It allows rapid development of robust, concise, correct and high-quality software.

## Koryaga features:

* guaranteed memory safety  
* referential transparency  
* minimal runtime

Travis CI status (Ubuntu GNU/Linux builds): [![Build Status](https://travis-ci.org/YoukaiCat/KLang.svg)](https://travis-ci.org/YoukaiCat/KLang)  
Appveyor CI status (Windows builds): [![Build status](https://ci.appveyor.com/api/projects/status/github/youkaicat/klang?branch=master&svg=true)](https://ci.appveyor.com/project/youkaicat/klang/branch/master)

## EBNF
```
program = "Начало",
          ( declaration | ( { declaration, ";" }, declaration ) ),
          { assignment },
          "Окончание" ;
declaration = ( "Анализ", identifier )
            | ( "Синтез", ( identifier | ( { identifier, "," }, identifier ) ) ) ;
assignment = identifier, "=", additive-expression ;
additive-expression = [ "-" ], multiplicative-expression, { ( "+" | "-" ), multiplicative-expression } ;
multiplicative-expression = boolean-binary-expression, { ( "*" | "/" ), boolean-binary-expression } ;
boolean-binary-expression = not-expression, { ( "И" | "ИЛИ" ), not-expression } ;
not-expression = ( "НЕ", base )
               | base ;
base = ( "(", expression, ")" )
     | identifier
     | real ;
identifier = letter, 3 * [ digit ] ;
letter = lower | upper
lower = "а" | "б" | "в" | "г" | "д" | "е" | "ё"
      | "ж" | "з" | "и" | "й" | "к" | "л" | "м"
      | "н" | "о" | "п" | "р" | "с" | "т" | "у"
      | "ф" | "х" | "ц" | "ч" | "ш" | "щ" | "ъ"
      | "ы" | "ъ" | "э" | "ю" | "я" ;
upper = "А" | "Б" | "В" | "Г" | "Д" | "Е" | "Ё"
      | "Ж" | "З" | "И" | "Й" | "К" | "Л" | "М"
      | "Н" | "О" | "П" | "Р" | "С" | "Т" | "У"
      | "Ф" | "Х" | "Ц" | "Ч" | "Ш" | "Щ" | "Ъ"
      | "Ы" | "Ъ" | "Э" | "Ю" | "Я" ;
real = number "." number ;
number = digit, { digit } ;
digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
```

## Свойства языка

### Приоретет операторов в порядке убывания:
1 - "НЕ"  
2 - "И", "ИЛИ"  
3 - "*", "/"  
4 - "+", "-"

### Ассоциативность
Левоассоциативные бинарные операторы: "И", "ИЛИ", "*", "/", "+", "-"  
Правоассоциативные унарные операторы: "НЕ", "-"

### Логические операции:
0.0 -- FALSE, любое число отличное от нуля -- TRUE  
Операторы "НЕ", "И", "ИЛИ" возвращают 1.0 если аргумент(ы) вычисляются в TRUE  
Оба оператора ленивые. Но так как выражение в скобках имеет приоретет выше, то:  
0.0 И (1.0 / 0.0) -> 0.0  
1.0 И (1.0 / 0.0) -> Ошибка: деление на ноль!  
1.0 ИЛИ (1.0 / 0.0) -> 1.0  
0.0 ИЛИ (1.0 / 0.0) -> Ошибка: деление на ноль!

### Регистр
Регистр символов имеет значение.

### Унарный минус
Для использования унарного минуса в выражениях, он должен находиться в самой левой части выражения.  
Скобки представляют собой отдельные выражения.  
Например: - 1 - (- 1 - 1) + (- 1) -- правильно. 5 - - 5 -- ошибка.

## Примеры
### Программа №1
```
Начало
Синтез а, б, б1
а = 1.0 + 1.0 + 1.0
б = а * 5.0
б = б - 3.0
Окончание
```
Вывод:
```
а = 3.0
б = 12.0
```

### Программа №2
```
Начало
Анализ а112;
Анализ б911;
Анализ в
в = 0.0 / 0.0 + 0.0
Окончание
```
Вывод:
```
Ошибка: деление на ноль!
```

### Программа №3
```
Начало
Синтез а, б, в;
Синтез а1, б1, в1
а1 = 0.0 ИЛИ 5.0 + 1.0
Окончание
```
Вывод:
```
а1 = 2.0
```

### Программа №4
```
Начало
Синтез П1
П1 = П1
Окончание
```
Вывод:
```
Ошибка: перед использованием переменной, ей должно быть присвоено значение!
```
