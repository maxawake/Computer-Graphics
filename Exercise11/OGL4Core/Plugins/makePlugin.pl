#!/usr/bin/perl

if ($#ARGV != 0) {
    die "usage: $0 <PluginName>";
}

my $temp = shift;
@pathparts = split /\//, $temp;
my $currdir = ".";
my $subdir = "../..";
my $PluginName = pop @pathparts;
foreach $part (@pathparts) {
    $currdir .= "/$part";
    if (! -d $currdir) {
        mkdir $currdir;
    }
    $subdir .= "/..";
}

system "cp -r Template $currdir/$PluginName";
system "rm -rf $currdir/$PluginName/.svn";
#system "xcopy /i Template $PluginName";

undef $/;   # slurp!

$SolGuid = `uuidgen`;
chomp $SolGuid;
$SolGuid =~ s/[\r\n]//g;
$ProjGuid = `uuidgen`;
chomp $ProjGuid;
$ProjGuid =~ s/[\r\n]//g;

chdir $currdir;
opendir(DIR, $PluginName);
while (my $file = readdir DIR) {
    #if ($file =~ /(Template\..*$|^Makefile$|^OGL4Plug.make$|^OGL4Plug.props$|^targetver.h$|^stdafx.h$)/) {
        my $newfile = $file;
        $newfile =~ s/Template/$PluginName/;
        rename "$PluginName/$file", "$PluginName/$newfile";
        open(IN, "$PluginName/$newfile");
        my $content = <IN>;
        close(IN);
        $content =~ s/\[Template\]/$PluginName/sg;
        $content =~ s/\[ProjectGuid\]/$ProjGuid/sg;
        $content =~ s/\[SolutionGuid\]/$SolGuid/sg;
        $content =~ s/\[OGL4CoreDirectory\]/$subdir/sg;
        open(OUT, ">$PluginName/$newfile");
        print OUT $content;
        close(OUT);
    #}
}
closedir(DIR);
