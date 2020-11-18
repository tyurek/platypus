#!/bin/bash
./kernelmonslow > base.txt
./imulbins/imul1x1&
./kernelmonslow > 1x1.txt
killall imul1x1
./imulbins/imul1x0&
./kernelmonslow > 1x0.txt
killall imul1x0
./imulbins/imul1xn1&
./kernelmonslow > 1xn1.txt
killall imul1xn1
./imulbins/imul0x1&
./kernelmonslow > 0x1.txt
killall imul0x1
./imulbins/imul0x0&
./kernelmonslow > 0x0.txt
killall imul0x0
./imulbins/imul0xn1&
./kernelmonslow > 0xn1.txt
killall imul0xn1
./imulbins/imuln1x1&
./kernelmonslow > n1x1.txt
killall imuln1x1
./imulbins/imuln1x0&
./kernelmonslow > n1x0.txt
killall imuln1x0
./imulbins/imuln1xn1&
./kernelmonslow > n1xn1.txt
killall imuln1xn1
./imulbins/imul654321x123456&
./kernelmonslow > 654321x123456.txt
killall imul654321x123456
