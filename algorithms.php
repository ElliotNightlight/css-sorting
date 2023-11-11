<?php

function BubbleSort($n, $cswap) {
    for($i = 0; $i < $n; $i++) {
        for($j = 0; $i + $j + 1 < $n; $j++) {
            $cswap($j, $j + 1);
        }
    }
}

function SelectionSort($n, $cswap) {
    for($i = 0; $i < $n; $i++) {
        for($j = $i + 1; $j < $n; $j++) {
            $cswap($i, $j);
        }
    }
}

function SortingNetworkTwoThree($n, $cswap) {
    for($i = $n-1; $i >= 1; $i--) {
        $delta = $i;
        while($delta % 2 == 0) $delta /= 2;
        while($delta % 3 == 0) $delta /= 3;
        if($delta > 1) continue;
        $delta = $i;
        for($j = 0; $j + $delta < $n; $j++) {
            $cswap($j, $j + $delta);
        }
    }
}

function Bitonic($n, $cswap) {
    $m = 1;
    while($m < $n) $m *= 2;
    for($k = 2; $k <= $m; $k *= 2) {
        for($i = 0; $i < $m; $i += $k) {
            $p = $i;
            $q = $i + $k - 1;
            while($p < $q) {
                if($p < $n and $q < $n)
                    $cswap($p, $q);
                $p++;
                $q--;
            }
        }

        for($l = floor($k/2); $l >= 2; $l = floor($l/2)) {
            for($i = 0; $i < $m; $i += $l) {
                $p = $i;
                $q = $i + floor($l/2);
                while($p < $i + $l and $q < $i + $l) {
                    if($p < $n and $q < $n)
                        $cswap($p, $q);
                    $p++;
                    $q++;
                }
            }
        }
    }
}

?>
