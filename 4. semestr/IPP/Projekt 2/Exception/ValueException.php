<?php


namespace IPP\Student\Exception;

use IPP\Core\ReturnCode;
use Throwable;
use IPP\Core\Exception\IPPException;

/**
 * Exception pro value
 */
class ValueException extends IPPException
{
    public function __construct(string $message, ?Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::VALUE_ERROR, $previous, false);
    }
}
