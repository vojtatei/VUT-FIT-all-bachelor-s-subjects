<?php

namespace IPP\Student;


// Třída pro převod escape sekvencí
class StringDecoder
{
    public static function decodeEscapeSequences(string $input): string
    {

        $input = preg_replace_callback('/\\\\(0[0-7][0-7])/', function ($matches) {
            $decimalCode = (int)$matches[1];
            if (($decimalCode >= 0 && $decimalCode <= 32) || $decimalCode == 35 || $decimalCode == 92) {

                return chr($decimalCode);
            } else if ($decimalCode >= 33 && $decimalCode <= 99) {

                return chr($decimalCode);
            }

            return "\\" . $matches[1];
        }, $input);

        return $input;
    }
}
