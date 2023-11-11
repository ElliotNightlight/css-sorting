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
    // to do !
}

?>
