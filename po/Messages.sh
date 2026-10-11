#!/bin/sh
# 提取可翻译字符串到 POT 文件
# 由 KDE CI / 手动执行：cd po && bash Messages.sh

DOMAIN="japanese-era-converter"

$XGETTEXT \
    `find .. -name \*.cpp -o -name \*.h` \
    -o $podir/${DOMAIN}.pot
