#!/usr/bin/perl
use warnings;
use strict;

my $board_size = @ARGV ? $ARGV[0] : 9;

print <<EOF
<?xml version="1.0" ?>
<GUILayout version="4">
<Window type="GlossySerpent/SimplePanel" name="gameWindow">
    <Property name="Position" value="{ {0, 0}, {0, 0} }"/>
    <Property name="Size" value="{ {1.0, 0}, {1.0, 0} }"/>
    <Window type="GlossySerpent/Button" name="back">
        <Property name="Text" value="Back"/>
        <Property name="Position" value="{ {0.9, 10.0}, {0.0, 10.0} }"/>
        <Property name="Size" value="{ {0.1, -20.0}, {0.1, 0.0} }"/>
    </Window>
    <Window type="GlossySerpent/SimplePanel" name="boardWindow">
        <Property name="Position" value="{ {0, 0}, {0.0, 0} }"/>
        <Property name="Size" value="{ {0.9, 0}, {0.9, 0} }"/>
EOF
;

my $n = $board_size - 1;;
for my $i (0..$n) {
    for my $j (0..$n) {
        my $x = 0.1111 * $j;
        my $y = 0.1111 * ($n - $i);
        print ' ' x 8, qq(<Window type="GlossySerpent/SerpentPanel" name="field_${i}_${j}">\n);
        print ' ' x 12, qq(<Property name="Position" value="{ {$x , 0}, {$y, 0} }"/>\n);
        print ' ' x 12, qq(<Property name="Size" value="{ {0.1111, 0}, {0.1111, 0} }"/>\n);
        print ' ' x 8, qq(</Window>\n);
    }
}

print <<EOF
    </Window>
</Window>
</GUILayout>
EOF
;

exit(0);
