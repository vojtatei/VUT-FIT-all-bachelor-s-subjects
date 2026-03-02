<?php


namespace IPP\Student\Exception;

use IPP\Core\ReturnCode;
use Throwable;
use IPP\Core\Exception\IPPException;

/**
 * Exception pro variable access
 */
class VariableAccessException extends IPPException
{
    public function __construct(string $message, ?Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::VARIABLE_ACCESS_ERROR, $previous, false);
    }
}
