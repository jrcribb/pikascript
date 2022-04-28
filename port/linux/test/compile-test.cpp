
#include "gtest/gtest.h"
extern "C" {
#include "PikaMain.h"
#include "PikaParser.h"
#include "PikaStdLib_MemChecker.h"
#include "PikaVM.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataStrs.h"
#include "pikaScript.h"
#include "pika_config_gtest.h"
#include "pikaCompiler.h"
}

extern char log_buff[LOG_BUFF_MAX][LOG_SIZE];


TEST(compiler, file1) {
    pikaCompileFile((char*)"test/python/main.py");
    EXPECT_EQ(pikaMemNow(), 0);
}


TEST(compiler, file) {
    char* lines =(char*)
        "len = __calls.len()\n"
        "mode = 'none'\n"
        "info_index = 0\n"
        "for i in range(0, len):\n"
        "    if len == 0:\n"
        "        break\n"
        "    if info_index == 0:\n"
        "        mode = __calls[i]\n"
        "        info_index = 1\n"
        "    elif info_index == 1:\n"
        "        if mode == 'always':\n"
        "            todo = __calls[i]\n"
        "            todo()\n"
        "            info_index = 0\n"
        "        elif mode == 'when':\n"
        "            when = __calls[i]\n"
        "            info_index = 2\n"
        "        elif mode == 'period_ms':\n"
        "            period_ms = __calls[i]\n"
        "            info_index = 2\n"
        "    elif info_index == 2:\n"
        "        if mode == 'when':\n"
        "            if when():\n"
        "                todo = __calls[i]\n"
        "                todo()\n"
        "            info_index = 0\n"
        "        elif mode == 'period_ms':\n"
        "            todo = __calls[i]\n"
        "            info_index = 3\n"
        "    elif info_index == 3:\n"
        "        if mode == 'period_ms':\n"
        "            if __tick > __calls[i]:\n"
        "                todo()\n"
        "                __calls[i] = __tick + period_ms\n"
        "            info_index = 0\n"
        "\n";
    pikaCompile((char*)"task.bin", (char*)lines);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(compiler, task) {
    char* lines =(char*)
        "len = __calls.len()\n"
        "mode = 'none'\n"
        "info_index = 0\n"
        "for i in range(0, len):\n"
        "    if len == 0:\n"
        "        break\n"
        "    if info_index == 0:\n"
        "        mode = __calls[i]\n"
        "        info_index = 1\n"
        "    elif info_index == 1:\n"
        "        if mode == 'always':\n"
        "            todo = __calls[i]\n"
        "            todo()\n"
        "            info_index = 0\n"
        "        elif mode == 'when':\n"
        "            when = __calls[i]\n"
        "            info_index = 2\n"
        "        elif mode == 'period_ms':\n"
        "            period_ms = __calls[i]\n"
        "            info_index = 2\n"
        "    elif info_index == 2:\n"
        "        if mode == 'when':\n"
        "            if when():\n"
        "                todo = __calls[i]\n"
        "                todo()\n"
        "            info_index = 0\n"
        "        elif mode == 'period_ms':\n"
        "            todo = __calls[i]\n"
        "            info_index = 3\n"
        "    elif info_index == 3:\n"
        "        if mode == 'period_ms':\n"
        "            if __tick > __calls[i]:\n"
        "                todo()\n"
        "                __calls[i] = __tick + period_ms\n"
        "            info_index = 0\n"
        "\n";

    Args buffs = {0};
    char* pikaAsm = Parser_multiLineToAsm(&buffs, (char*)lines);

    ByteCodeFrame bytecode_frame;
    byteCodeFrame_init(&bytecode_frame);
    byteCodeFrame_appendFromAsm(&bytecode_frame, pikaAsm);
    /* do something */
    byteCodeFrame_print(&bytecode_frame);
    printf("Asm size: %d\r\n", strGetSize(pikaAsm));

    byteCodeFrame_printAsArray(&bytecode_frame);

    /* deinit */
    byteCodeFrame_deinit(&bytecode_frame);
    strsDeinit(&buffs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(compiler, demo1) {
    char* lines = (char*)"append(__val)";
    Parser_compilePyToBytecodeArray(lines);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(compiler, snake_file) {
    char* lines =(char*)
        "from PikaObj import *\n"
        "import PikaStdLib\n"
        "import machine \n"
        "\n"
        "# hardware init\n"
        "lcd = machine.LCD()\n"
        "lcd.init()\n"
        "lcd.clear('white')\n"
        "key = machine.KEY()\n"
        "key.init()\n"
        "time = machine.Time()\n"
        "x_max = 120\n"
        "y_max = 150\n"
        "\n"
        "# snake init\n"
        "s = machine.Point()\n"
        "w = 9\n"
        "h = 9\n"
        "s.x = 50\n"
        "s.y = 10\n"
        "len = 0\n"
        "while len < 3:\n"
        "    b = s\n"
        "    i = 0\n"
        "    while i < len:\n"
        "        b = b.next\n"
        "        i = i + 1\n"
        "    b.next = machine.Point()\n"
        "    b.next.x = b.x - 10\n"
        "    b.next.y = b.y\n"
        "    b.next.prev = b\n"
        "    len = len + 1\n"
        "# ring link\n"
        "b.next = s\n"
        "s.prev = b\n"
        "\n"
        "i = 0\n"
        "b = s\n"
        "while i < len:\n"
        "    lcd.fill(b.x, b.y, w, h, 'blue')\n"
        "    b = b.next\n"
        "    i = i + 1\n"
        "\n"
        "print('snake lengh')\n"
        "print(len)\n"
        "\n"
        "# fruit init\n"
        "f = machine.Point()\n"
        "f.x = 30\n"
        "f.y = 20\n"
        "lcd.fill(f.x, f.y, w, h, 'green')\n"
        "\n"
        "# memory check\n"
        "mem = PikaStdLib.MemChecker()\n"
        "print('mem used max:')\n"
        "mem.max()\n"
        "\n"
        "# main loop\n"
        "d = 0\n"
        "isUpdate = 1\n"
        "isEat = 0\n"
        "while True:\n"
        "    if isUpdate:\n"
        "        # isUpdate = 0\n"
        "        # check eat fruit\n"
        "        if f.x == s.x and f.y == s.y:\n"
        "            # have eat fruit\n"
        "            isEat = 1\n"
        "            f.x = f.x + 30\n"
        "            if f.x > x_max:\n"
        "                f.x = f.x - x_max\n"
        "            f.y = f.y + 30\n"
        "            if f.y > y_max:\n"
        "                f.y = f.y - y_max\n"
        "            lcd.fill(f.x, f.y, w, h, 'green')\n"
        "        # move snake by the direction\n"
        "        if d == 0:\n"
        "            x_new = s.x + 10\n"
        "            y_new = s.y\n"
        "            if x_new > x_max:\n"
        "                x_new = 0\n"
        "        elif d == 1:\n"
        "            x_new = s.x\n"
        "            y_new = s.y - 10\n"
        "            if y_new < 0:\n"
        "                y_new = y_max\n"
        "        elif d == 2:\n"
        "            x_new = s.x\n"
        "            y_new = s.y + 10\n"
        "            if y_new > y_max:\n"
        "                y_new = 0\n"
        "        elif d == 3:\n"
        "            x_new = s.x - 10\n"
        "            y_new = s.y\n"
        "            if x_new < 0:\n"
        "                x_new = x_max\n"
        "        if isEat:\n"
        "            isEat = 0\n"
        "            b_new = machine.Point()\n"
        "            b_new.x = x_new\n"
        "            b_new.y = y_new\n"
        "            b_new.prev = s.prev\n"
        "            b_new.next = s\n"
        "            s.prev.next = b_new\n"
        "            s.prev = b_new\n"
        "            s = b_new\n"
        "            len = len + 1\n"
        "            print('snake lengh')\n"
        "            print(len)\n"
        "            print('mem used max:')\n"
        "            mem.max()\n"
        "        # drow the snake and fruit\n"
        "        # clear last body\n"
        "        lcd.fill(s.prev.x, s.prev.y, w, h, 'white')\n"
        "        # new body\n"
        "        s.prev.x = x_new\n"
        "        s.prev.y = y_new\n"
        "        # head is last body\n"
        "        s = s.prev\n"
        "        lcd.fill(s.x, s.y, w, h, 'blue')\n"
        "        b = s\n"
        "        i = 0\n"
        "    # scan key\n"
        "    key_val = key.get()\n"
        "    if key_val == 0:\n"
        "        d = 0\n"
        "        isUpdate = 1\n"
        "    elif key_val == 1:\n"
        "        d = 1\n"
        "        isUpdate = 1\n"
        "    elif key_val == 2:\n"
        "        d = 2\n"
        "        isUpdate = 1\n"
        "    elif key_val == 3:\n"
        "        d = 3\n"
        "        isUpdate = 1\n";
    pikaCompile((char*)"snake.bin", (char*)lines);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(compiler, import_bf_mem) {
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    char* lines =(char*)
        "def mytest():\n"
        "    print('test')\n"
        "\n";
    ByteCodeFrame bf;
    byteCodeFrame_init(&bf);
    bytecodeFrame_fromMultiLine(&bf, lines);
    obj_importModuleWithByteCodeFrame(pikaMain, (char*)"mtest", &bf);
    byteCodeFrame_deinit(&bf);
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(compiler, import_bf1) {
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    char* lines =(char*)
        "def mytest():\n"
        "    print('test_import_bf1')\n"
        "\n";
    ByteCodeFrame bf;
    byteCodeFrame_init(&bf);
    bytecodeFrame_fromMultiLine(&bf, lines);
    obj_importModuleWithByteCodeFrame(pikaMain, (char*)"mtest", &bf);
    obj_run(pikaMain, (char*)
            "mtest.mytest()\n"
            "\n"
    );
    EXPECT_STREQ(log_buff[0], "test_import_bf1\r\n");
    byteCodeFrame_deinit(&bf);
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(compiler, import_bf2) {
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    char* lines =(char*)
        "class Test:\n"
        "    def mytest(self):\n"
        "        print('test_import_bf2')\n"
        "\n";
    ByteCodeFrame bf;
    byteCodeFrame_init(&bf);
    bytecodeFrame_fromMultiLine(&bf, lines);
    obj_importModuleWithByteCodeFrame(pikaMain, (char*)"mtest", &bf);
    obj_run(pikaMain, (char*)
            "m = mtest.Test()\n"
            "m.mytest()\n"
            "\n"
    );
    byteCodeFrame_deinit(&bf);
    obj_deinit(pikaMain);
    EXPECT_STREQ(log_buff[0], "test_import_bf2\r\n");
    EXPECT_EQ(pikaMemNow(), 0);
}