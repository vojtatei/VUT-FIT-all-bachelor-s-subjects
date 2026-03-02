<?php


namespace IPP\Student\Exception;

use IPP\Core\ReturnCode;
use Throwable;
use IPP\Core\Exception\IPPException;

/**
 * Exception pro operand type
 */
class OperandTypeException extends IPPException
{
    public function __construct(string $message, ?Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::OPERAND_TYPE_ERROR, $previous);
    }
}
