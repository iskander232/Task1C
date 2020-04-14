# Task1C

## Условие

Реализуйте программу для определения позиции одного изображения внутри другого. На вход поступают две картинки: одна -- полная и вторая -- прямоугольная “вырезка” из первой меньшего размера.

Программа должна вернуть координаты второго изображения внутри первого. Достаточно левого верхнего угла второй картинки. Либо программа может дать пользователю ориентир в виде: “юг”, “северо-восток”, “центр изображения” и т.п. Тогда подготовка ответа должна занимать меньше времени. Какой ответ хочет получить пользователь: точный или ориентир - стоит задавать через входной параметр.

Изображение воспринимаем как двумерный массив целых чисел, от 0 до 255 включительно.

Пример:
Первая картинка:

[[0, 1, 2],

[3, 4, 5],

[6, 7, 8]]

Вторая картинка:

[[0, 1],

[3, 4]]

Программа должна вернуть либо [0, 0], либо “северо-запад”. Не забывайте, что  во втором случае программа должна работать быстрее. 

## Решение в случае если нас просят найти левый верхний угол

Каждую картинку будем хранить в специальном классе `Picture`. Сделаем метод `Rotate`, чтобы 4 раза поворачивать вторую картинку и считать, что они ориентированы одинаково. Пусть первая картинка с размерами N1 x M1, а вторая N2 x M2. 

Воспользуемся алгоритмом Ахо-Корасик: 

Засунем в бор все горизонтальные подстроки длины `M2` из 1ой картинки(Асимптотика О(N1 x M1 x M2))

По очереди "скармливаем" бору строки из 2ой картинки и запоминаем координаты пикселей, где они начинаются(Асимптотика О(N2 x M2 + N1 x M1) то есть суммарная длина всех строк "скармливаемых" бору и ответа)

Далее эти пиксели это начала рядов из 2ой картинки, то есть надо найти столбец, где они идут подряд. Для этого все начала 0ого ряда засунем в сет. Для каждого пикселя из следуюего ряда смотрим, лежит ли предыдущий пиксель в сете, и, если лежит, то засунем его в новый сет. Когда обработаем все начала ряда Х, то заменим старый сет на новый(Асимптотика О(N1 x M1 x log(N1 x M1)))

То есть в итоге мы получили решение за кубическую сложность

## Решение, где надо искать направление

Я не справился(

## Как запускать

`Task` это Cmake проект. После сборки получится исполняемый файл `Task`, который ожидает входные данные в таком виде:

N1 M1

картинка1 с размерами N1 x M1

N2 M2

картинка с размерами N2 x M2

# Пример запуска:

```
./Task

2 3
1 2 3
4 5 6

1 2
4 5

Ответ: 1 0

```
