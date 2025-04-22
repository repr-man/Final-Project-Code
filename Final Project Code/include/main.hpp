#pragma once

class Main {
    class SafeExit {};
public:
    void main();

    [[noreturn]] static void safeExit();
};
