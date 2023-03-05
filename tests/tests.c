#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../src/exe.h"

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(quarantedeuxSH, onlyecho, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, onlyechowithsemicolon, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo ;");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, simpletest, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo oui");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("oui\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, if_then_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if true; then echo yes; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("yes\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, if_then_else_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if false; then echo yes; else echo no; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("no\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, if_then_elif_else_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if false; then echo yes; elif false; then echo maybe; else echo no; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("no\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, compound_list_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo yes; echo no");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("yes\nno\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, complex_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if true; then echo yes; elif false; then echo maybe; else echo no; fi; echo end");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("yes\nend\n");
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, if_then_error_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if false; else echo a; fi");
    fflush(stdout);

    int expected = 2; // expected error code
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

Test(quarantedeuxSH, if_then_valid_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if echo VALID; else echo a; fi");
    fflush(stdout);

    int expected = 2; // expected error code
    cr_assert_eq(actual, expected, "Expected %d. Got %d.", expected, actual);
}

/*
Test(quarantedeuxSH, invalid_command_test)
{
    int actual = executeCommand("notacommand");
    fflush(stdout);

    int expected = 127;
    cr_assert(WEXITSTATUS(actual) == expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, compound_list_error_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo yes; notacommand");
    fflush(stdout);

    int expected = 127;
    //cr_assert_stdout_eq_str("yes\n");
    cr_assert(WEXITSTATUS(actual) == expected, "Expected exit code of %d, got %d", expected, actual);
} */

Test(quarantedeuxSH, complex_valid_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if true; then echo yes; elif false; then notacommand; else echo no; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("yes\n");
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, if_then_nested_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if true; then if true; then echo yes; fi; fi");
    fflush(stdout);
    
    int expected = 0;
    cr_assert_stdout_eq_str("yes\n");
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, if_then_nested_else_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if true; then if false; then echo yes; else echo no; fi; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("no\n");
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, if_then_multiple_conditions_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if true; then echo yes; elif true; then echo maybe; else echo no; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("yes\n");
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, if_then_multiple_conditions_else_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if false; then echo yes; elif false; then echo maybe; else echo no; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("no\n");
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, if_then_multiple_compound_list_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if true; then echo yes; echo no; elif false; then echo maybe; else echo no; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("yes\nno\n");
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, if_then_multiple_compound_list_else_test, .init = redirect_all_stdout)
{
    int actual = executeCommand("if false; then echo yes; echo no; elif false; then echo maybe; else echo no; echo end; fi");
    fflush(stdout);

    int expected = 0;
    cr_assert_stdout_eq_str("no\nend\n");
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, true_, .init = redirect_all_stdout)
{
    int actual = executeCommand("true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, false_, .init = redirect_all_stdout)
{
    int actual = executeCommand("false");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, true_semi_false, .init = redirect_all_stdout)
{
    int actual = executeCommand("true; false");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, false_semi_true, .init = redirect_all_stdout)
{
    int actual = executeCommand("false; true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, false_true, .init = redirect_all_stdout)
{
    int actual = executeCommand("false true");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, true_false, .init = redirect_all_stdout)
{
    int actual = executeCommand("true false");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, not_true, .init = redirect_all_stdout)
{
    int actual = executeCommand("! true");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, not_false, .init = redirect_all_stdout)
{
    int actual = executeCommand("! false");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_and_1, .init = redirect_all_stdout)
{
    int actual = executeCommand("true && false");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_and_2, .init = redirect_all_stdout)
{
    int actual = executeCommand("false && false");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_and_3, .init = redirect_all_stdout)
{
    int actual = executeCommand("true && true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_and_4, .init = redirect_all_stdout)
{
    int actual = executeCommand("false && true");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_or_1, .init = redirect_all_stdout)
{
    int actual = executeCommand("true || false");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_or_2, .init = redirect_all_stdout)
{
    int actual = executeCommand("false || true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_or_3, .init = redirect_all_stdout)
{
    int actual = executeCommand("true || true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, basic_or_4, .init = redirect_all_stdout)
{
    int actual = executeCommand("false || false");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, negativity_and_combo_1, .init = redirect_all_stdout)
{
    int actual = executeCommand("! false && ! true");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, negativity_and_combo_2, .init = redirect_all_stdout)
{
    int actual = executeCommand("! false && true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, negativity_and_combo_3, .init = redirect_all_stdout)
{
    int actual = executeCommand("false && ! true");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, negativity_or_combo_1, .init = redirect_all_stdout)
{
    int actual = executeCommand("! false || true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, negativity_or_combo_2, .init = redirect_all_stdout)
{
    int actual = executeCommand("! false || ! true");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, negativity_or_combo_3, .init = redirect_all_stdout)
{
    int actual = executeCommand("false || ! true");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, Diversification_and_or_2, .init = redirect_all_stdout)
{
    int actual = executeCommand("false || ! true && false || true && ! false");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
}

Test(quarantedeuxSH, negativity_or_combo_4, .init = redirect_all_stdout)
{
    int actual = executeCommand("false || echo a || echo b");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\n");
}

Test(quarantedeuxSH, negativity_and_combo_4, .init = redirect_all_stdout)
{
    int actual = executeCommand("true && echo a && echo b");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\nb\n");
}

Test(quarantedeuxSH, negativity_or_combo_5, .init = redirect_all_stdout)
{
    int actual = executeCommand("false || echo a || ! echo b");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\n");
}

Test(quarantedeuxSH, negativity_and_combo_5, .init = redirect_all_stdout)
{
    int actual = executeCommand("true && echo a && ! echo b");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\nb\n");
}

Test(quarantedeuxSH, simple_or_4, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a || echo b");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\n");
}

Test(quarantedeuxSH, simple_or_3, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a || ! echo b");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\n");
}

Test(quarantedeuxSH, simple_or_2, .init = redirect_all_stdout)
{
    int actual = executeCommand("! echo a || echo b");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\nb\n");
}

Test(quarantedeuxSH, simple_or_1, .init = redirect_all_stdout)
{
    int actual = executeCommand("! echo a || ! echo b");
    fflush(stdout);

    int expected = 1;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\nb\n");
}

Test(quarantedeuxSH, multy_echos, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a; echo b; echo c; echo d; echo e");
    fflush(stdout);

    int expected = 0;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("a\nb\nc\nd\ne\n");
}

Test(quarantedeuxSH, bad_and_and, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a && &&");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_beginning_and, .init = redirect_all_stdout)
{
    int actual = executeCommand("&&");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_beginning_or, .init = redirect_all_stdout)
{
    int actual = executeCommand("||");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_ending_and, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a &&");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_ending_or, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a ||");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_or_or, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a || ||");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_or_pipe, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a || |");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_or_semicolon, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a || ;");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}

Test(quarantedeuxSH, bad_and_semicolon, .init = redirect_all_stdout)
{
    int actual = executeCommand("echo a && ;");
    fflush(stdout);

    int expected = 2;
    cr_assert_eq(actual, expected, "Expected exit code of %d, got %d", expected, actual);
    cr_assert_stdout_eq_str("");
}
