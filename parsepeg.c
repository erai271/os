enum {
    P_grammar,
    P_rule,
    P_pattern,
    P_alternative,
    P_lookop,
    P_lookahead,
    P_countop,
    P_suffix,
    P_primary,
    P_any,
    P_literal,
    P_class,
    P_call,
    P_identifier,
    P_sp,
}

tag_to_str(tag: int): *byte {
    if tag == P_grammar { return "grammar"; }
    if tag == P_rule { return "rule"; }
    if tag == P_pattern { return "pattern"; }
    if tag == P_alternative { return "alternative"; }
    if tag == P_lookop { return "lookop"; }
    if tag == P_lookahead { return "lookahead"; }
    if tag == P_countop { return "countop"; }
    if tag == P_suffix { return "suffix"; }
    if tag == P_primary { return "primary"; }
    if tag == P_any { return "any"; }
    if tag == P_literal { return "literal"; }
    if tag == P_class { return "class"; }
    if tag == P_call { return "call"; }
    if tag == P_identifier { return "identifier"; }
    if tag == P_sp { return "sp"; }
    die("invalid tag");
}

p_grammar(c: *peg): int {
    var ok: int;
    enter(c);
    ok = p_sp(c);
    if ok {
    ok = p_rule(c);
    if ok {
    loop {
    choice(c);
    ok = p_rule(c);
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    }
    if ok {
    choice(c);
    ok = any(c);
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    }
    if ok { leave(c, P_grammar); } else { fail(c); }
    return ok;
}

p_rule(c: *peg): int {
    var ok: int;
    enter(c);
    ok = p_identifier(c);
    if ok {
    ok = p_sp(c);
    }
    if ok {
    ok = literal(c, "<-");
    }
    if ok {
    ok = p_sp(c);
    }
    if ok {
    ok = p_pattern(c);
    }
    if ok { leave(c, P_rule); } else { fail(c); }
    return ok;
}

p_pattern(c: *peg): int {
    var ok: int;
    enter(c);
    ok = p_alternative(c);
    if ok {
    loop {
    choice(c);
    ok = literal(c, "/");
    if ok {
    choice(c);
    ok = literal(c, "/");
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    }
    if ok {
    ok = p_sp(c);
    }
    if ok {
    ok = p_alternative(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, P_pattern); } else { fail(c); }
    return ok;
}

p_alternative(c: *peg): int {
    var ok: int;
    enter(c);
    ok = p_lookahead(c);
    if ok {
    loop {
    choice(c);
    ok = p_lookahead(c);
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, P_alternative); } else { fail(c); }
    return ok;
}

p_lookop(c: *peg): int {
    var ok: int;
    enter(c);
    ok = charset(c, "!&");
    if ok { leave(c, P_lookop); } else { fail(c); }
    return ok;
}

p_lookahead(c: *peg): int {
    var ok: int;
    enter(c);
    choice(c);
    ok = p_lookop(c);
    if ok {
    ok = p_sp(c);
    }
    if ok { commit(c); } else { ok = 1; }
    if ok {
    ok = p_suffix(c);
    }
    if ok { leave(c, P_lookahead); } else { fail(c); }
    return ok;
}

p_countop(c: *peg): int {
    var ok: int;
    enter(c);
    ok = charset(c, "*+?");
    if ok { leave(c, P_countop); } else { fail(c); }
    return ok;
}

p_suffix(c: *peg): int {
    var ok: int;
    enter(c);
    ok = p_primary(c);
    if ok {
    loop {
    choice(c);
    ok = p_countop(c);
    if ok {
    ok = p_sp(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, P_suffix); } else { fail(c); }
    return ok;
}

p_primary(c: *peg): int {
    var ok: int;
    enter(c);
    choice(c);
    ok = literal(c, "(");
    if ok {
    ok = p_sp(c);
    }
    if ok {
    ok = p_pattern(c);
    }
    if ok {
    ok = literal(c, ")");
    }
    if !ok { choice(c);
    ok = p_any(c);
    }
    if !ok { choice(c);
    ok = p_any(c);
    }
    if !ok { choice(c);
    ok = p_literal(c);
    }
    if !ok { choice(c);
    ok = p_literal(c);
    }
    if !ok { choice(c);
    ok = p_class(c);
    }
    if !ok { choice(c);
    ok = p_class(c);
    }
    if !ok { choice(c);
    ok = p_call(c);
    }
    if !ok { choice(c);
    ok = p_call(c);
    }
    if ok { commit(c); } else { fail(c); }
    if ok {
    ok = p_sp(c);
    }
    if ok { leave(c, P_primary); } else { fail(c); }
    return ok;
}

p_any(c: *peg): int {
    var ok: int;
    enter(c);
    ok = literal(c, ".");
    if ok { leave(c, P_any); } else { fail(c); }
    return ok;
}

p_literal(c: *peg): int {
    var ok: int;
    enter(c);
    ok = charset(c, "'");
    if ok {
    loop {
    choice(c);
    choice(c);
    ok = charset(c, "'");
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    if ok {
    ok = any(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok {
    ok = charset(c, "'");
    }
    if ok { leave(c, P_literal); } else { fail(c); }
    return ok;
}

p_class(c: *peg): int {
    var ok: int;
    enter(c);
    ok = literal(c, "[");
    if ok {
    loop {
    choice(c);
    choice(c);
    ok = literal(c, "]");
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    if ok {
    choice(c);
    ok = any(c);
    if ok {
    ok = literal(c, "-");
    }
    if ok {
    ok = any(c);
    }
    if !ok { choice(c);
    ok = any(c);
    }
    if !ok { choice(c);
    ok = any(c);
    }
    if ok { commit(c); } else { fail(c); }
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok {
    ok = literal(c, "]");
    }
    if ok { leave(c, P_class); } else { fail(c); }
    return ok;
}

p_call(c: *peg): int {
    var ok: int;
    enter(c);
    ok = p_identifier(c);
    if ok {
    choice(c);
    ok = p_sp(c);
    if ok {
    ok = literal(c, "<-");
    }
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    }
    if ok { leave(c, P_call); } else { fail(c); }
    return ok;
}

p_identifier(c: *peg): int {
    var ok: int;
    enter(c);
    ok = charset(c, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz");
    if ok {
    loop {
    choice(c);
    ok = charset(c, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz");
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, P_identifier); } else { fail(c); }
    return ok;
}

p_sp(c: *peg): int {
    var ok: int;
    enter(c);
    loop {
    choice(c);
    choice(c);
    ok = charset(c, "\x09\x0a\x0d\x20");
    if !ok { choice(c);
    ok = literal(c, "//");
    if ok {
    loop {
    choice(c);
    choice(c);
    ok = charset(c, "\x0a\x0d");
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    if ok {
    ok = any(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    }
    if !ok { choice(c);
    ok = literal(c, "//");
    if ok {
    loop {
    choice(c);
    choice(c);
    ok = charset(c, "\x0a\x0d");
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    if ok {
    ok = any(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    }
    if ok { commit(c); } else { fail(c); }
    if !ok { ok = 1; break; }
    commit(c);
    }
    if ok { leave(c, P_sp); } else { fail(c); }
    return ok;
}
