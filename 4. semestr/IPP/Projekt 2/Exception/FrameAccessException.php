<?php

namespace IPP\Student\Exception;

use IPP\Core\ReturnCode;
use Throwable;
use IPP\Core\Exception\IPPException;

/**
 * Exception pro Frame access error
 */
class FrameAccessException extends IPPException
{
    public function __construct(string $message, ?Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::FRAME_ACCESS_ERROR, $previous, false);
    }
}
