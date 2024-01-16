BEGIN {
    seen_first = 0
    can_process = 0
    show_help = 0
}
/^\+\+/ && !seen_first {
    seen_first = 1
    next
}
/^\+\+/ && seen_first && !can_process {
    can_process = 1
}

# show full "Usage" text only once
/^Usage/ && !show_help {
    print $0
    print "  [...]"
    can_process = 0
    next
}
/ -h$/ {
    show_help = 1
}
/^Usage/ {
    show_help = 0
}

# this needs to be after all actiosn that modify "can_process" and in particular set it to 1
!can_process {
    next
}

/^\+\+/ {
    if (cmd) {
        $1 = ""
        cmd = cmd " |" $0
    }
    else {
        $1 = "]$"
        cmd = $0
    }
    next
}
{
    if (cmd) {
        print ""
        print cmd
        cmd = ""
    }

    print $0
}
