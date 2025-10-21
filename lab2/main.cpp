#include <algorithm>
#include <iostream>
#include "UnixConsole.h"
#include "Command.h"

// TODO:
/*
  * Загрузить вселенную
  * Universe uni;
  * ofstream f("file.txt")
  * f << ui;
  * f >> ui;
  */

// TODO: Sound processor
/*
 * Громче тише, эхо. .wav файл
 * Там 16-битный инты, которые означают значения волны! 44 кГц 16 бит
 *  ------------ wav файл ------------
 *  1. заголовок с инфой
 *  2. массив интов
 *  ----------------------------------
 *
 *  увеличить громкость: *2. std::clamp -- чтобы не переполнялось при умножении (или сложени!!)
 *  посчитать в 32bit. а потом перевести в 16-bit.
 *
 * преобразование фурье: b * sin(ax). любой сигнал можно апроксимировать суммой синусов и косинусов
 * что можно после фурье делать?
 *
 * про фабрику
 * есть effect: от него наследуются
 * volume, mute, deleay, distrotion
*/
int main() {
    Console *term = new UnixConsole();
    while (true) {
        term->Repeat();
    }
    return 0;
}