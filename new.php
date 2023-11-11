<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Pick your favourite sorting algorithm</title>
    <!--This document should preferably avoid using backtick as strings including backticks will inform the compiler where to insert generated code.-->

    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Ubuntu+Mono&family=Ubuntu:wght@400;700&display=swap" rel="stylesheet">

    <!--Useful style: https://www.realtimecolors.com/?colors=ffffff-220426-d6d600-00a8dc-ae00ab&fonts=Poppins-Poppins-->
    <link rel="stylesheet" href="style.css">

</head>
<body>
    <div id="page">
        <h1>Pick your favourite <span>sorting</span> algorithm</h1>
        <p>Here, you can choose your favourite sorting algorithm and the array to be sorted. Having done so, click the button to see CSS sort your array.i</p>

        <h2>Parameters</h2>
        <form action="index.php" method="get">
            <select name="algo">
                <option value="bubble">Bubble sort</option>
                <option value="selection">Selection sort</option>
                <option value="sn23">SN-23</option>
            </select>
            <input type="text" name="data" value="3 1 4 1 5">
            <button type="submit">Sort</button>
        </form>
    </div>
</body>
</html>

<!-- http://localhost:8000/index.php?algo=sn23&data=3+1+4+1+5+9+2+6+5+3+5+8+9+7+9+3+2+3+8+4+6+2+6 -->
