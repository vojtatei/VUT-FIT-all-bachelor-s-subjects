<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Modules/Admin/templates/@layout.latte */
final class Template209133569c extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		['head' => 'blockHead', 'main' => 'blockMain'],
	];


	public function main(): array
	{
		extract($this->params);
		echo "\n";
		if ($this->getParentName()) {
			return get_defined_vars();
		}
		$this->renderBlock('head', get_defined_vars()) /* line 3 */;
		echo '

';
		$this->renderBlock('main', get_defined_vars()) /* line 6 */;
		echo "\n";
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		$this->parentName = '../../Base/templates/@layout.latte';
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {block #head} on line 3 */
	public function blockHead(array $ʟ_args): void
	{
		
	}


	/** {block #main} on line 6 */
	public function blockMain(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		echo '
	<div class="container-fluid">
		<div class="row">
			<nav id="sidebarMenu" class="col-md-3 col-lg-2 d-md-block bg-light sidebar">
				<div class="sidebar-sticky">
					<ul class="nav flex-column">
						<li class="nav-item">
							<a class="nav-link active" href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(':Front:Home:default')) /* line 14 */;
		echo '">
								Hlavní stránka
							</a>
						</li>
						<li class="nav-item">
							<a class="nav-link active" href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(':Admin:Home:default')) /* line 19 */;
		echo '">
								Dashboard
							</a>
						</li>
						<li class="nav-item">
							<a class="nav-link" href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(':Admin:User:default')) /* line 24 */;
		echo '">
								Uživatelé
							</a>
						</li>
						<li class="nav-item">
							<a class="nav-link" href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(':Admin:Conference:default')) /* line 29 */;
		echo '">
								Konference
							</a>
            			</li>
						<li class="nav-item">
							<a class="nav-link" href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(':Admin:Presentation:default')) /* line 34 */;
		echo '">
								Prezentace
							</a>
						</li>
						<li class="nav-item">
							<a class="nav-link" href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(':Admin:Reservation:default')) /* line 39 */;
		echo '">
								Rezervace
							</a>
						</li>
					</ul>
				</div>
			</nav>
			<main role="main" class="col-md-9 ml-sm-auto col-lg-10 px-4">
';
		$this->renderBlock('content', [], 'html') /* line 47 */;
		echo '			</main>
		</div>
	</div>
';
	}

}
