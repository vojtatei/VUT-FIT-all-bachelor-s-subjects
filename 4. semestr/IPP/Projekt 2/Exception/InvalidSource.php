<?php

namespace IPP\Student\Exception;

use IPP\Core\ReturnCode;
use Throwable;
use IPP\Core\Exception\IPPException;

/**
 * Exception pro invalid source
 */
class InvalidSource extends IPPException
{
    public function __construct(string $message, ?Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::INVALID_SOURCE_STRUCTURE, $previous, false);
    }
}
