enum {
    PEG_grammar,
    PEG_rule,
    PEG_pattern,
    PEG_alternative,
    PEG_lookop,
    PEG_lookahead,
    PEG_countop,
    PEG_suffix,
    PEG_primary,
    PEG_any,
    PEG_literal,
    PEG_class,
    PEG_call,
    PEG_identifier,
    PEG_sp,
}

tag_to_str(tag: int): *byte {
    if tag == PEG_grammar { return "grammar"; }
    if tag == PEG_rule { return "rule"; }
    if tag == PEG_pattern { return "pattern"; }
    if tag == PEG_alternative { return "alternative"; }
    if tag == PEG_lookop { return "lookop"; }
    if tag == PEG_lookahead { return "lookahead"; }
    if tag == PEG_countop { return "countop"; }
    if tag == PEG_suffix { return "suffix"; }
    if tag == PEG_primary { return "primary"; }
    if tag == PEG_any { return "any"; }
    if tag == PEG_literal { return "literal"; }
    if tag == PEG_class { return "class"; }
    if tag == PEG_call { return "call"; }
    if tag == PEG_identifier { return "identifier"; }
    if tag == PEG_sp { return "sp"; }
    die("invalid tag");
}

peg_PEG_grammar(c: *peg): int {
    var ok: int;
    enter(c, PEG_grammar);
    ok = peg_PEG_sp(c);
    if ok {
    ok = peg_PEG_rule(c);
    if ok {
    loop {
    choice(c);
    ok = peg_PEG_rule(c);
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
    if ok { leave(c, PEG_grammar); } else { fail(c); }
    return ok;
}

peg_PEG_rule(c: *peg): int {
    var ok: int;
    enter(c, PEG_rule);
    ok = peg_PEG_identifier(c);
    if ok {
    ok = peg_PEG_sp(c);
    }
    if ok {
    ok = literal(c, "<-");
    }
    if ok {
    ok = peg_PEG_sp(c);
    }
    if ok {
    ok = peg_PEG_pattern(c);
    }
    if ok { leave(c, PEG_rule); } else { fail(c); }
    return ok;
}

peg_PEG_pattern(c: *peg): int {
    var ok: int;
    enter(c, PEG_pattern);
    ok = peg_PEG_alternative(c);
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
    ok = peg_PEG_sp(c);
    }
    if ok {
    ok = peg_PEG_alternative(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, PEG_pattern); } else { fail(c); }
    return ok;
}

peg_PEG_alternative(c: *peg): int {
    var ok: int;
    enter(c, PEG_alternative);
    ok = peg_PEG_lookahead(c);
    if ok {
    loop {
    choice(c);
    ok = peg_PEG_lookahead(c);
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, PEG_alternative); } else { fail(c); }
    return ok;
}

peg_PEG_lookop(c: *peg): int {
    var ok: int;
    enter(c, PEG_lookop);
    ok = charset(c, "!&");
    if ok { leave(c, PEG_lookop); } else { fail(c); }
    return ok;
}

peg_PEG_lookahead(c: *peg): int {
    var ok: int;
    enter(c, PEG_lookahead);
    choice(c);
    ok = peg_PEG_lookop(c);
    if ok {
    ok = peg_PEG_sp(c);
    }
    if ok { commit(c); } else { ok = 1; }
    if ok {
    ok = peg_PEG_suffix(c);
    }
    if ok { leave(c, PEG_lookahead); } else { fail(c); }
    return ok;
}

peg_PEG_countop(c: *peg): int {
    var ok: int;
    enter(c, PEG_countop);
    ok = charset(c, "*+?");
    if ok { leave(c, PEG_countop); } else { fail(c); }
    return ok;
}

peg_PEG_suffix(c: *peg): int {
    var ok: int;
    enter(c, PEG_suffix);
    ok = peg_PEG_primary(c);
    if ok {
    loop {
    choice(c);
    ok = peg_PEG_countop(c);
    if ok {
    ok = peg_PEG_sp(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, PEG_suffix); } else { fail(c); }
    return ok;
}

peg_PEG_primary(c: *peg): int {
    var ok: int;
    enter(c, PEG_primary);
    choice(c);
    ok = literal(c, "(");
    if ok {
    ok = peg_PEG_sp(c);
    }
    if ok {
    ok = peg_PEG_pattern(c);
    }
    if ok {
    ok = literal(c, ")");
    }
    if !ok { choice(c);
    ok = peg_PEG_any(c);
    }
    if !ok { choice(c);
    ok = peg_PEG_literal(c);
    }
    if !ok { choice(c);
    ok = peg_PEG_class(c);
    }
    if !ok { choice(c);
    ok = peg_PEG_call(c);
    }
    if ok { commit(c); } else { fail(c); }
    if ok {
    ok = peg_PEG_sp(c);
    }
    if ok { leave(c, PEG_primary); } else { fail(c); }
    return ok;
}

peg_PEG_any(c: *peg): int {
    var ok: int;
    enter(c, PEG_any);
    ok = literal(c, ".");
    if ok { leave(c, PEG_any); } else { fail(c); }
    return ok;
}

peg_PEG_literal(c: *peg): int {
    var ok: int;
    enter(c, PEG_literal);
    ok = literal(c, "'");
    if ok {
    loop {
    choice(c);
    choice(c);
    ok = literal(c, "'");
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    if ok {
    ok = any(c);
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok {
    ok = literal(c, "'");
    }
    if ok { leave(c, PEG_literal); } else { fail(c); }
    return ok;
}

peg_PEG_class(c: *peg): int {
    var ok: int;
    enter(c, PEG_class);
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
    if ok { commit(c); } else { fail(c); }
    }
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok {
    ok = literal(c, "]");
    }
    if ok { leave(c, PEG_class); } else { fail(c); }
    return ok;
}

peg_PEG_call(c: *peg): int {
    var ok: int;
    enter(c, PEG_call);
    ok = peg_PEG_identifier(c);
    if ok {
    choice(c);
    ok = peg_PEG_sp(c);
    if ok {
    ok = literal(c, "<-");
    }
    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }
    }
    if ok { leave(c, PEG_call); } else { fail(c); }
    return ok;
}

peg_PEG_identifier(c: *peg): int {
    var ok: int;
    enter(c, PEG_identifier);
    ok = charset(c, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz");
    if ok {
    loop {
    choice(c);
    ok = charset(c, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz");
    if !ok { ok = 1; break; }
    commit(c);
    }
    }
    if ok { leave(c, PEG_identifier); } else { fail(c); }
    return ok;
}

peg_PEG_sp(c: *peg): int {
    var ok: int;
    enter(c, PEG_sp);
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
    if ok { commit(c); } else { fail(c); }
    if !ok { ok = 1; break; }
    commit(c);
    }
    if ok { leave(c, PEG_sp); } else { fail(c); }
    return ok;
}
