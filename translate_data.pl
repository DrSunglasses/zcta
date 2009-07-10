#!/usr/bin/perl -w
use strict;
use warnings;
use diagnostics;
use English '-no_match_vars';
use DirHandle;

mkdir 'pdata', 0755;

my $datadir = DirHandle->new('data/');
my @numbers = ();

my $filename;
while (defined($filename = $datadir->read)) {
	if ($filename =~ /zt(\d\d)_d00a.dat/) {
		push @numbers, $1;
	}
}

for my $number (@numbers) {
	print "$number\n";
	system ("./translate.exe", 'data/zt' . $number . '_d00a.dat', 'data/zt' . $number. '_d00.dat', 'pdata/'. $number . '.dat');
}
