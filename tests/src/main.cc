#include "test_manager.h"

int main(void) {
    TestManager manager = TestManager();

    manager.run_tests();
    return 0;
}
