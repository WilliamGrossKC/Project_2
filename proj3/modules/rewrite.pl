#!/usr/local/bin/perl

use strict;
use warnings;

while (my $line = <>) {
  if ($line =~ /^(\s*)ret(\s*#.*)?$/) {
    print "$1mv\tx26, ra\n";
    print "$1and\tx26, x26, x30\n";
    print "$1or\tx26, x26, x28\n";
    print "$1jr\tx26\n"
  } elsif ($line =~ /^(\s*)call\s+(\w+)(\s*#.*)?$/) {
    print "$1la\tra, $2\n";
    print "$1mv\tx26, ra\n";
    print "$1and\tx26, x26, x30\n";
    print "$1or\tx26, x26, x28\n";
    print "$1jalr\tx26\n"
  } elsif ($line =~ /^(\s*)tail\s+(\w+)(\s*#.*)?$/) {
    print "$1la\tx6, $2\n";
    print "$1mv\tx26, x6\n";
    print "$1and\tx26, x26, x30\n";
    print "$1or\tx26, x26, x28\n";
    print "$1jr\tx26\n"
  } elsif ($line =~ /^(\s*)jalr\s+(\w+)(\s*#.*)?$/) {
    if ($2 eq "x26" or $2 eq "s10") {
      print $line;
    } else {
      print "$1mv\tx26, $2\n";
      print "$1and\tx26, x26, x30\n";
      print "$1or\tx26, x26, x28\n";
      print "$1jalr\tx26\n"
    }
  } elsif ($line =~ /^(\s*)l(b|bu|h|hu|w|wu|d)\s+(\w+),\s+(\S+)\((\w+)\)(\s*#.*)?$/) {
    if ($4 eq "0" and ($5 eq "x27" or $5 eq "s11")) {
      print $line;
    } else {
      print "$1addi\tx27, $5, $4\n";
      print "$1and\tx27, x27, x30\n";
      print "$1or\tx27, x27, x29\n";
      print "$1l$2\t$3, 0(x27)\n"
    }
  } elsif ($line =~ /^(\s*)s(b|bu|h|hu|w|wu|d)\s+(\w+),\s+(\S+)\((\w+)\)(\s*#.*)?$/) {
    if ($4 eq "0" and ($5 eq "x27" or $5 eq "s11")) {
      print $line;
    } else {
      print "$1addi\tx27, $5, $4\n";
      print "$1and\tx27, x27, x30\n";
      print "$1or\tx27, x27, x29\n";
      print "$1s$2\t$3, 0(x27)\n"
    }
  } else {
    print $line;
  }
}
