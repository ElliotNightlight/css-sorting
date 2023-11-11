<?php
require_once('algorithms.php');
if(!isset($_GET['algo'])) $_GET['algo'] = 'bubble';
if(!isset($_GET['data'])) $_GET['data'] = '3 1 4 1 5';

$algorithm = $_GET['algo'];
$numbers = @array_map('intval', explode(' ', $_GET['data']));
if(!is_array($numbers)) $numbers = [3, 1, 4, 1, 5];
$length = count($numbers);

$minimum = min($numbers);
$maximum = max($numbers);
$minimum = min(1, $minimum);
$maximum = max(10, $maximum);
$alpha = 1 / ($maximum - $minimum) * 9 / 10;
$gamma = -1 * $minimum / ($maximum - $minimum) * 9 / 10 + 1/10;

$pivotal_code = "";
$visuals_code = "";
$lookup = [];
$C = 0;

$frames = [];
$major_step = 7;
$minor_step = 1;
$total_steps = 0;

function map($x) {
    global $alpha;
    global $gamma;
    global $minimum;
    global $maximum;
    if($gamma == 0) return "calc($alpha * clamp($minimum, $x, $maximum) * var(--theater-height))";
    else return "calc(($alpha * clamp($minimum, $x, $maximum) + $gamma) * var(--theater-height))";
}

$cswap = function ($p, $q) {
    global $pivotal_code;
    global $lookup;
    global $C;

    global $frames;
    global $major_step;
    global $minor_step;
    global $total_steps;

    $pivotal_code .= "    --t" . ($C + 1) . ": min(var(--t{$lookup[$p]}), var(--t{$lookup[$q]}));\n";
    $pivotal_code .= "    --t" . ($C + 2) . ": max(var(--t{$lookup[$p]}), var(--t{$lookup[$q]}));\n";

    $frames[$p][] = [$total_steps, 'tertiary', map("var(--t{$lookup[$p]})")];
    $frames[$p][] = [$total_steps + $minor_step, 'primary', map("var(--t{$lookup[$p]})")];
    $total_steps += $major_step;
    $frames[$p][] = [$total_steps - $minor_step, 'secondary', map("var(--t" . ($C + 1) . ")")];
    $frames[$p][] = [$total_steps, 'tertiary', map("var(--t" . ($C + 1) . ")")];

    $total_steps -= $major_step;

    $frames[$q][] = [$total_steps, 'tertiary', map("var(--t{$lookup[$q]})")];
    $frames[$q][] = [$total_steps + $minor_step, 'primary', map("var(--t{$lookup[$q]})")];
    $total_steps += $major_step;
    $frames[$q][] = [$total_steps - $minor_step, 'secondary', map("var(--t" . ($C + 2) . ")")];
    $frames[$q][] = [$total_steps, 'tertiary', map("var(--t" . ($C + 2) . ")")];

    $lookup[$p] = $C + 1;
    $lookup[$q] = $C + 2;
    $C += 2;
};

switch($algorithm) {
    case 'selection': $pivotal_code .= "/* Selection sort */\n"; break;
    case 'sn23': $pivotal_code .= "/* SN-23 */\n"; break;
    default:
    case 'bubble': $pivotal_code .= "/* Bubble sort */\n"; break;
}
$pivotal_code .= ":root {\n";
for($i = 0; $i < $length; $i++) {
    $pivotal_code .= "    --a$i: {$numbers[$i]};\n";

    $frames[$i] = [];
    $frames[$i][] = [0, 'tertiary', map("var(--a$i)")];
}

$pivotal_code .= "\n";
for($i = 0; $i < $length; $i++) {
    $lookup[$i] = $C;
    $C++;
    $pivotal_code .= "    --t{$lookup[$i]}: var(--a$i);\n";
}
switch($algorithm) {
    case 'selection': SelectionSort($length, $cswap); break;
    case 'sn23': SortingNetworkTwoThree($length, $cswap); break;
    default:
    case 'bubble': BubbleSort($length, $cswap); break;
}

$pivotal_code .= "\n";
for($i = 0; $i < $length; $i++) {
    $pivotal_code .= "    --b$i: var(--t{$lookup[$i]});\n";

    $frames[$i][] = [$total_steps, 'tertiary', map("var(--b$i)")];
}
$pivotal_code .= "}\n";

for($i = 0; $i < $length; $i++) {
    $visuals_code .= "#theater > div:nth-child(" . ($i + 1) . ") { height: " . map("var(--a$i)") . "; animation-name: animation" . ($i + 1) . "; }\n";
    $visuals_code .= "@keyframes animation" . ($i + 1) . " {\n";
    $last = -1;
    foreach($frames[$i] as $elm) {
        [$steps, $color, $height] = $elm;
        if($steps == $last) continue;
        $last = $steps;
        $visuals_code .= "    " . (100.0 * $steps / max($total_steps, 1)) . "% { background-color: ";
        if($color == 'tertiary') $visuals_code .= "var(--color-tertiary);";
        else if($color == "primary") $visuals_code .= "var(--color-primary);";
        else if($color == "secondary") $visuals_code .= "var(--color-secondary);";
        $visuals_code .= " height: $height;";
        $visuals_code .= " }\n";
    }
    $visuals_code .= "}\n";
}

$visuals_code .= "\n";
for($i = 0; $i < $length; $i++) {
    $visuals_code .= "#a span:nth-child(" . ($i+1) . "):after { counter-reset: c var(--a$i); content: counter(c); }\n";
    $visuals_code .= "#b span:nth-child(" . ($i+1) . "):after { counter-reset: c var(--b$i); content: counter(c); }\n";
}

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Sorting your array with CSS magic</title>
    <!--This document should preferably avoid using backtick as strings including backticks will inform the compiler where to insert generated code.-->

    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Ubuntu+Mono&family=Ubuntu:wght@400;700&display=swap" rel="stylesheet">

    <!--Useful style: https://www.realtimecolors.com/?colors=ffffff-220426-d6d600-00a8dc-ae00ab&fonts=Poppins-Poppins-->
    <link rel="stylesheet" href="style.css">
    <style>
        #a, #b {
            user-select: none;
        }

        #theater {
            --theater-width:  990px;
            --theater-height: 440px;
            --n: <?= "$length" ?>;
            --bar-gap:   calc(var(--theater-width) / (var(--n) + (var(--n) - 1) * 0.25) * 0.25);
            --bar-width: calc(var(--theater-width) / (var(--n) + (var(--n) - 1) * 0.25)       );

            width:  var(--theater-width);
            height: var(--theater-height);

            display: flex;
            flex-direction: row;
            gap: var(--bar-gap);
            align-items: end;
        }

        #theater > div {
            background-color: var(--color-tertiary);
            width:  var(--bar-width);
            height: var(--theater-height);

            animation-duration: <?= $total_steps / $major_step ?>s;
            animation-fill-mode: forwards;
        }
    </style>

    <style>
<?= $pivotal_code . "\n" . $visuals_code ?>
    </style>

</head>
<body>
    <div id="page">
        <h1>Sorting your array with <span>CSS</span> magic</h1>
        <p>What you can see below is pure CSS integer array sorting. (Well, there's a button, which uses JS, to reload the animation but the sorting and animation themselves are done with pure CSS.) So, sit tight and enjoy your browser doing excessive work to sort an array of a few numbers. By the way, this page has been generated with PHP and you can <a href="new.php">generate a new one</a> with different numbers and sorting algorithm.</p>
        <pre class="box okay">Compilation successful. Sorting now...</pre>

        <h2>The sorting itself</h2>
        <p class="box" id="a"><?= str_repeat("<span></span> ", $length) ?></p>
        <p class="box" id="b"><?= str_repeat("<span></span> ", $length) ?></p>

        <div id="theater"><?= str_repeat("<div></div>", $length) ?></div>
        <button id="reload" onclick="restartAnimation()">Reload</button>

        <h2>The pivotal code</h2>
        <pre class="box"><?= $pivotal_code ?></pre>
    </div>
    <script>
        function restartAnimation() {
            const x = document.querySelector('div#theater');
            x.innerHTML = x.innerHTML;
        }
    </script>
</body>
</html>
