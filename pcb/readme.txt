2x4057-beta.dip -- разводка 2x CD4057+ 1x HC595 для управления.
На плате нет, но все выходы HC595 притянуты к земле через 10-15КОм резисторы (иначе у меня почему-то не завелось).

waterer-beta.dip -- файл с основными "мозгами". не могу назвать разводку удачной из-за необходимости делать два провода внавес:
1-й от разъёма ICSP до RESET, второй -- между +Vcc и выводом WP. остальное расписано в текст рядом со схемой.
Из неочевидного: диод Шоттки используется, чтобы ионистор, подключённый после диода, при потере питания основной схемы,
питал только часы. У меня используются ионисторы на 0.22Ф и 0.1Ф. хватает на недельку где-то.
