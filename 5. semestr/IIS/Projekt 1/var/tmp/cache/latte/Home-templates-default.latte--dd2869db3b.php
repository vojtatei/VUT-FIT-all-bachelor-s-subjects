<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Modules/Front/Home/templates/default.latte */
final class Templatedd2869db3b extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		['slider' => 'blockSlider', 'content' => 'blockContent'],
	];


	public function main(): array
	{
		extract($this->params);
		if ($this->getParentName()) {
			return get_defined_vars();
		}
		$this->renderBlock('slider', get_defined_vars()) /* line 1 */;
		echo '

';
		$this->renderBlock('content', get_defined_vars()) /* line 15 */;
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {block slider} on line 1 */
	public function blockSlider(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		echo '	<div id="carouselExampleSlidesOnly" class="carousel slide" data-ride="carousel">
		<div class="carousel-inner h-100">
			<div class="carousel-item active">
				<img class="d-block w-100" src="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 5 */;
		echo '/img/slider.jpg" alt="First slide">
				<div class="overlay h-100">
					<h2 class="overlay-text">Cesta za vzděláním začíná</h2>
					<h2 class="jerabek">ZDE</h2>
				</div>
			</div>
		</div>
	</div>
';
	}


	/** {block content} on line 15 */
	public function blockContent(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		echo '
	<h2 class="text-center">Konference v blízké době</h2>
';
		/* line 18 */ $_tmp = $this->global->uiControl->getComponent("soonConferences");
		if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
		$_tmp->render();
		echo '
	<h2 class="text-center">Nejpopulárnější prezentace</h2>
';
		/* line 21 */ $_tmp = $this->global->uiControl->getComponent("popularPresentations");
		if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
		$_tmp->render();
		
	}

}
