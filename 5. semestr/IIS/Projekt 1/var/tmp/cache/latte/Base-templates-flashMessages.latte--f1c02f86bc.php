<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Modules/Base/templates/flashMessages.latte */
final class Templatef1c02f86bc extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		['flashMessages' => 'blockFlashMessages'],
	];


	public function main(): array
	{
		extract($this->params);
		echo '<style>
  .flash {
    padding: 10px;
    margin-bottom: 10px;
    border-radius: 5px;
    opacity: 1; /* Výchozí neprůhlednost */
    transition: opacity 5s; /* Přechod pro změnu neprůhlednosti */
    text-align: center;
  }

  .flash.success {
    background-color: #dff0d8;
    color: #3c763d;
  }

  .flash.error {
    background-color: #f2dede;
    color: #a94442;
  }

  .flash.info {
    background-color: #d9edf7;
    color: #31708f;
  }
</style>

';
		if ($this->getParentName()) {
			return get_defined_vars();
		}
		$this->renderBlock('flashMessages', get_defined_vars()) /* line 28 */;
		echo "\n";
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		if (!$this->getReferringTemplate() || $this->getReferenceType() === "extends") {
			foreach (array_intersect_key(['flash' => '29'], $this->params) as $ʟ_v => $ʟ_l) {
				trigger_error("Variable \$$ʟ_v overwritten in foreach on line $ʟ_l");
			}
		}
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {block flashMessages} on line 28 */
	public function blockFlashMessages(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		$iterations = 0;
		foreach ($flashes as $flash) /* line 29 */ {
			echo '        <div class="flash ';
			echo LR\Filters::escapeHtmlAttr($flash->type) /* line 30 */;
			echo '">
            ';
			echo LR\Filters::escapeHtmlText($flash->message) /* line 31 */;
			echo '
        </div>
';
			$iterations++;
		}
		echo '
    <script>
      document.addEventListener(\'DOMContentLoaded\', function () {
        setTimeout(function () {
          var flashes = document.querySelectorAll(\'.flash\');
          flashes.forEach(function (flash) {
            flash.style.transition = \'opacity 0.5s\';
            flash.style.opacity = \'0\';
            setTimeout(function () {
              flash.remove();
            }, 500); // Čas pro dokončení přechodu
          });
        }, 5000); // Čas před skrytím zprávy (3000ms = 3 sekundy)
      });
    </script>
';
	}

}
