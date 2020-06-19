<?php
$WORD_SIZE = 4;

$fn1=__DIR__.'/'.$argv[1];
$fn2=$argv[2];
$in=file_get_contents($fn1);

function Encode($in) {
	global $WORD_SIZE;
	$a=str_split($in,$WORD_SIZE);
	$counts=Array();
	foreach ($a as $word) {
		if (!isset($counts[$word])) {
			$counts[$word]=0;
		}
		$counts[$word]++;
	}
	arsort($counts);
	$dict=Array();
	$i=0;
	foreach ($counts as $k=>$v) {
		$dict[]=$k;
		$index[$k]=$i;
		$i++;
	}
	$AD=Array();
	foreach ($a as $word) {
		$I=$index[$word];
		$data=decbin($I);
		$d=str_split($data);
		$data=implode('',array_reverse($d));
		$bufLen=intval((strlen($data)+1)/2);
		$ADRec=str_repeat('0',$bufLen);
		$ADRec.='1';
		$ADRec.=$data;
		$AD[]=$ADRec;
	}

	$dict=implode('',$dict);
	$sl=strlen($dict);
	$dl='';
	for ($x=0;$x<4;$x++) {
		$dl.=chr($sl%256);
		$sl/=256;
		$sl=intval($sl);
	}
	$dict="$dl$dict";
	$AD=implode('',$AD);
	while (strlen($AD)%8) $AD.='0';

	$out=Array();
	for($i=0; $i<strlen($AD); $i+=8) {
		$out[]= chr(intval(substr($AD, $i, 8), 2));
	}
	$AD=implode('',$out);

	$rv="$dict$AD";
	return($rv);
}
function pullRec(&$AD) {
	$z=0;
	$l=strlen($AD);
	while (($z<$l)&&($AD[$z]==0)) $z++;
	$Z=$z*2;
	$ZT=$Z+$z+1;
	$rec=substr($AD,0,$ZT);
	if ($rec[strlen($rec)-1]=='0') $Z--;
	$ZT=$Z+$z+1;
	$data=substr($AD,$z+1,$Z);
	$data=strrev($data);
	$index=bindec($data);
	$AD=substr($AD,$ZT);
	return($index);
}

function Decode($in) {
	global $WORD_SIZE;
	$DL=substr($in,0,4);
	$dl=0;
	for ($x=0;$x<4;$x++) {
		$dl*=256;
		$dl+=ord($DL[3-$x]);
	}
	$dict=substr($in,4,$dl);
	$dict=str_split($dict,$WORD_SIZE);
	$ad=str_split(substr($in,$dl+4));
	$AD=Array();
	foreach ($ad as $b) {
		$B=decbin(ord($b));
		while (strlen($B)<8) $B="0$B";
		$AD[]=$B;
	}
	$AD=implode('',$AD);
	$O=Array();
	do {
		$index=pullRec($AD);
		$O[]=$dict[$index];
	} while (strlen($AD));
	$rv=implode('',$O);
	return($rv);
}

if (stripos($fn1,'.xiv')) {
	$out=Decode($in);
} else {
	while (strlen($in)%$WORD_SIZE) $in.='0';
	$out=Encode($in);
}
file_put_contents(__DIR__.'/'.$fn2,$out);
Echo('done');
?>