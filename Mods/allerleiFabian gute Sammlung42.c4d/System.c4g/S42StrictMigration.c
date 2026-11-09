// Compatibility at boundaries that used the former Strict 1 and 2 value rules.
#strict 3

global func S42MigrationEqual(a, b)
{
    if (a == b) return true;
    var ta = GetType(a), tb = GetType(b);
    if (ta == C4V_Any) return (tb == C4V_Int || tb == C4V_Bool || tb == C4V_C4ID) && !b;
    if (tb == C4V_Any) return (ta == C4V_Int || ta == C4V_Bool || ta == C4V_C4ID) && !a;
    if (ta == C4V_Int && (tb == C4V_Bool || tb == C4V_C4ID)) return Equal(a, b);
    if (tb == C4V_Int && (ta == C4V_Bool || ta == C4V_C4ID)) return Equal(a, b);
    return false;
}

global func S42MigrationStep(&slot, int delta, bool postfix)
{
    var before = slot;
    if (!before) before = 0;
    slot = before + delta;
    if (postfix) return before;
    return slot;
}

global func S42MigrationAssign(&slot, string op, int value)
{
    if (!slot) slot = 0;
    if (op == "+=") return slot += value;
    if (op == "-=") return slot -= value;
    if (op == "*=") return slot *= value;
    if (op == "/=") return slot /= value;
    if (op == "%=") return slot %= value;
    if (op == "**=") return slot **= value;
    if (op == "<<=") return slot <<= value;
    if (op == ">>=") return slot >>= value;
    if (op == "&=") return slot &= value;
    if (op == "|=") return slot |= value;
    if (op == "^=") return slot ^= value;
}

global func S42MigrationFirst(value, ...)
{
    return value;
}
