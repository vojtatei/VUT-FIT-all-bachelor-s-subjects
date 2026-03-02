<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Modules/Admin/Presentation/templates/default.latte */
final class Template862aa4dbc6 extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		['content' => 'blockContent'],
	];


	public function main(): array
	{
		extract($this->params);
		if ($this->getParentName()) {
			return get_defined_vars();
		}
		$this->renderBlock('content', get_defined_vars()) /* line 1 */;
		echo "\n";
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {block #content} on line 1 */
	public function blockContent(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		if (isset($conferenceName)) /* line 2 */ {
			echo '		<h2>Prezentace pro konferenci: ';
			echo LR\Filters::escapeHtmlText($conferenceName) /* line 3 */;
			echo '</h2>
';
			/* line 4 */ $_tmp = $this->global->uiControl->getComponent("grid");
			if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
			$_tmp->render();
			echo '		<a href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiPresenter->link(":Admin:Conference:default")) /* line 5 */;
			echo '" class="btn btn-secondary mt-3">Zpět na seznam konferencí</a>
';
		} else /* line 6 */ {
			echo '		<h2>Prezentace</h2>
';
			/* line 8 */ $_tmp = $this->global->uiControl->getComponent("grid");
			if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
			$_tmp->render();
		}
		echo '	

	
';
	}

}
