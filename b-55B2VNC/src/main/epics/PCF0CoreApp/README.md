# PCF0CORE — две платы NI5772 и NI6683H

| Устройство | Порт | Ресурс по умолчанию | Префикс PV |
|---|---|---|---|
| DiamondDetectorDAQ №0, Ch0/Ch1 | B-0 | RIO0 | D1-I2-HWCF:B-0- |
| NI6683H | B-1 | BOARD_NUM=0 | D1-I2-HWCF:B-1- |
| DiamondDetectorDAQ №1, Ch0/Ch1 | B-2 | RIO1 | D1-I2-HWCF:B-2- |

NI5763, его DAN-подписка и старые PCF0 DB/подстановки удалены. Системная диагностика IOC и шаблоны NI6683H сохранены. Другие IOC и исходный драйвер NI5763 не изменены.

## До первого аппаратного испытания

Текущая проверка в Windows: 26 Python-тестов прошли; аппаратный запуск, сборка C++/IOC и визуальная проверка CSS не выполнены. Не считать исходники проверенной поставкой до следующих шагов.

1. На CODAC выполнить `make test`, затем `make` в `src/main/c++/nds3-ni5772`. Проверить, что SDK/runtime удовлетворяют требованию NI-RIO 26.0 из экспортированного заголовка. Убедиться, что собран `nds3-6683h`.
2. Собрать EPICS-проект штатным способом; из `src/main/epics` — `make`. Проверить отсутствие ошибок NDS/asyn при загрузке.
3. Сверить физические NI RIO resources двух плат. Перед запуском можно задать `NI5772_0_RESOURCE`, `NI5772_1_RESOURCE`, `NI6683_BOARD_NUM` в окружении. Путь к битфайлу задан в `iocBoot/iocD1-I2-PCF0CORE/envUser` относительно TOP; при иной раскладке установки изменить его.
4. Запустить `mvn run -Dioc=PCF0CORE` из каталога `b-55B2VNC`. Обе NI5772 создаются с INIT=OFF, без автоматического сбора.
5. На CSS-клиенте установить `EPICS_CA_MAX_ARRAY_BYTES=4194304`, открыть `src/main/boy/mimics/PCF0CORE-DiamondDetectorDAQ.opi` (также добавлен пункт Measurements → PCF0 Diamond DAQ).

## Первичная проверка

Для одной платы нажать ON и проверить Status/WorkerFault. Задать порог, гейт и параметры канала; начальные RawEnable и ScopeEnable равны 0. Нажать ARM/RUN: ожидаются Acquiring=1, WorkerFault=0 и готовый DeviceConfig. ARM разрешает внешний PXI_Trig0; он может запустить оба канала. Для программного запуска нажать Start channel.

Пример Channel Access для B-0 после ON:

```sh
caput D1-I2-HWCF:B-0-GateTicks 200000
caput D1-I2-HWCF:B-0-Threshold_Ch0 160
caput D1-I2-HWCF:B-0-ScopeEnable_Ch0 1
caput D1-I2-HWCF:B-0-StateMachine-setState 7
caput D1-I2-HWCF:B-0-Start_Ch0 1
caget D1-I2-HWCF:B-0-Run_Ch0
caget D1-I2-HWCF:B-0-WorkerFault
caget D1-I2-HWCF:B-0-SpecLost_Ch0
```

Проверить скорости и форму импульса, SpectrumBadBlocks/ScopeBadWords, потери и SpecOverrun. Сравнить спектры с импульсными записями на малой загрузке. Проверить OFF → ON → RUNNING, изменение шкал на границе гейта, внешний триггер, одновременную работу двух плат и остановку каждой независимо. Raw включать отдельно при контроле потерь и загрузки хоста.

Остановка: корневая команда ON (4), затем OFF (1), если нужно закрыть сессию. Start=0 не гарантирует остановку после внешнего триггера. WorkerFault требует OFF → ON → RUNNING; числовое состояние StateMachine само по себе не подтверждает работу потока.

HMI показывает последний блок/гейт. Между PV нет транзакционной согласованности; FPGA timestamps не синхронизированы с UTC NI6683H автоматически. Включение поддержки NI6683H не настраивает физическую разводку PXI_Trig0 за оператора.

## Сопровождение

- `Db/DiamondDetectorDAQ.substitutions`: два устройства и четыре канала, включая узкие I16 границы Ch1.
- `dbToLoad.cmd`: только Diamond, NI6683H и IOC health.
- `userPreDriverConf.cmd`: библиотеки и три экземпляра.
- OPI: обзор, `DiamondDetectorDAQ-Board.opi`, `DiamondDetectorDAQ-Channel.opi`.
- Генератор DB/OPI: `src/main/c++/nds3-ni5772/tools/generate_ioc_ui.py`.

Удалены старые исходные DB, но ранее установленные файлы в сборочном `db/` могут сохраниться до чистой установки; новый IOC их не загружает. При регенерации из SDD сначала согласовать/ретрофитить изменения: старые SDD XML автоматически не обновлялись и могут восстановить прежние сгенерированные DB и startup-файлы.
