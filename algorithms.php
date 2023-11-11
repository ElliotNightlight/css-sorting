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
    $magic = [];
    $k = 1;
    while($k < $n) {
        $l = 1;
        while($l < $n) {
            if($k * $l < $n)
                $magic[] = $k * $l;
            $l *= 3;
        }
        $k *= 2;
    }

    for($i = count($magic)-1; $i >= 0; $i--) {
        $delta = $magic[$i];
        for($j = 0; $j + $delta < $n; $j++) {
            $cswap($j, $j + $delta);
        }
    }
}

?>
