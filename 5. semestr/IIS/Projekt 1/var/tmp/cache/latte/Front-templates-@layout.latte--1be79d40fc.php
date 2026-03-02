<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Modules/Front/templates/@layout.latte */
final class Template1be79d40fc extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		['head' => 'blockHead', 'main' => 'blockMain', 'slider' => 'blockSlider', 'content' => 'blockContent'],
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
		$this->renderBlock('main', get_defined_vars()) /* line 9 */;
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
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		echo '	<link rel="stylesheet" href="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 4 */;
		echo '/assets/front.css" defer>
	<script src="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 5 */;
		echo '/assets/front.js" defer></script>

';
	}


	/** {block #main} on line 9 */
	public function blockMain(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		echo '

<nav class="navbar navbar-expand-lg navbar-dark py-4">
	<div class="container">
		<a class="navbar-brand" href="#">EDU-<span class=\'jerabek\'>KONF</span></a>

		<button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav"
				aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
			<span class="navbar-toggler-icon"></span>
		</button>

		<div class="collapse navbar-collapse" id="navbarNav">
			<ul class="navbar-nav ms-auto align-items-center">
				<li class="nav-item">
					<a href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:Home:")) /* line 24 */;
		echo '"';
		echo ($ʟ_tmp = array_filter([$presenter->isLinkCurrent(':Front:Home:') ? 'active' : null, 'nav-link', 'me-4', 'text-white'])) ? ' class="' . LR\Filters::escapeHtmlAttr(implode(" ", array_unique($ʟ_tmp))) . '"' : "" /* line 24 */;
		echo '>Domů</a>
				</li>
				<li class="nav-item">
					<a href="';
		echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:Conference:default")) /* line 27 */;
		echo '"';
		echo ($ʟ_tmp = array_filter([$presenter->isLinkCurrent(':Admin:Home:') ? 'active' : null, 'nav-link', 'me-4', 'text-white'])) ? ' class="' . LR\Filters::escapeHtmlAttr(implode(" ", array_unique($ʟ_tmp))) . '"' : "" /* line 27 */;
		echo '>Konference</a>
				</li>
';
		if ($user->isLoggedIn()) /* line 29 */ {
			echo '				  <li class="nav-item dropdown">
					<a class="nav-link dropdown-toggle text-white" href="#" id="navbarDropdown" role="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
					  Profil
					</a>
					<div class="dropdown-menu" aria-labelledby="navbarDropdown">
					<a href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:User:myConferences")) /* line 35 */;
			echo '"';
			echo ($ʟ_tmp = array_filter(['dropdown-item', 'me-4'])) ? ' class="' . LR\Filters::escapeHtmlAttr(implode(" ", array_unique($ʟ_tmp))) . '"' : "" /* line 35 */;
			echo '>Moje konference</a>
					<a href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:Reservations:default")) /* line 36 */;
			echo '"';
			echo ($ʟ_tmp = array_filter(['dropdown-item', 'me-4'])) ? ' class="' . LR\Filters::escapeHtmlAttr(implode(" ", array_unique($ʟ_tmp))) . '"' : "" /* line 36 */;
			echo '>Moje rezervace</a>
					<a href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:User:myPresentations")) /* line 37 */;
			echo '"';
			echo ($ʟ_tmp = array_filter(['dropdown-item', 'me-4'])) ? ' class="' . LR\Filters::escapeHtmlAttr(implode(" ", array_unique($ʟ_tmp))) . '"' : "" /* line 37 */;
			echo '>Moje prezentace</a>
					<a href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:User:default")) /* line 38 */;
			echo '"';
			echo ($ʟ_tmp = array_filter(['dropdown-item', 'me-4'])) ? ' class="' . LR\Filters::escapeHtmlAttr(implode(" ", array_unique($ʟ_tmp))) . '"' : "" /* line 38 */;
			echo '>Správa profilu</a>
					  <div class="dropdown-divider"></div>
					  <a class="dropdown-item" href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:Sign:out")) /* line 40 */;
			echo '">Odhlásit se</a>
					</div>
				  </li>
				</div>
				<li class="nav-item">
					<a href="#" class="btn btn-success mx-2 me-4" data-bs-toggle="modal" data-bs-target="#dialog-conf-create">Vytvořit konferenci</a>
				</li>
';
		} else /* line 47 */ {
			echo '				<li class="nav-item">
						<a class="btn btn-success mx-2" href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:Sign:in")) /* line 49 */;
			echo '">Přihlásit se</a>
				</li>
				<li class="nav-item">
						<a class="btn btn-primary mx-2" href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Front:Sign:up")) /* line 52 */;
			echo '">Registrovat se</a>
				</li>
';
		}
		echo "\n";
		if ($user->isInRole('admin')) /* line 56 */ {
			echo '					<a href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link(":Admin:Home:")) /* line 57 */;
			echo '"';
			echo ($ʟ_tmp = array_filter([$presenter->isLinkCurrent(':Admin:Home:') ? 'active' : null, 'nav-link', 'me-4'])) ? ' class="' . LR\Filters::escapeHtmlAttr(implode(" ", array_unique($ʟ_tmp))) . '"' : "" /* line 57 */;
			echo '>Admin</a>
';
		}
		echo '			</ul>
		</div>
	</div>
</nav>

';
		$this->renderBlock('slider', get_defined_vars()) /* line 64 */;
		echo '


	<div class="container py-4">
		<main role="main" class="inner cover">
';
		$this->renderBlock('content', get_defined_vars()) /* line 70 */;
		echo '
		</main>
	</div>


<footer>
    <div class="container py-5">
        <div class="row gy-3">
            <div class="col-md">
                <div class="contact-info">
                    <h3 class="mb-2">Buďme v kontaktu</h3>
                    <div class="d-flex align-items-center my-3">
                        <i class="fa fa-envelope contact-icon circle-icon"></i>
                        <div>
                            <div>Email</div>
                            <div>info@edu-konf.cz</div>
                        </div>
                    </div>
                    <div class="d-flex align-items-center">
                        <i class="fa fa-map contact-icon circle-icon"></i>
                        <div>
                            <div>Adresa</div>
                            <div>Brno, Česká republika</div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="text-center mt-4 text-secondary">
            <small>&copy; 2024 Edu-Konf. Všechna práva vyhrazena. Autoři: Jakub Jeřábek (xjerab28), Doubravka Šimůnková (xsimun05), Vojtěch Teichmann (xteich02)</small>
        </div>
    </div>
</footer>


<div class="modal fade" id="dialog-conf-create" tabindex="-1" aria-labelledby="dialog-label" aria-hidden="true">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<h5 class="modal-title" id="dialog-label">Vytvořit konferenci</h5>
				<button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Zavřít">
				</button>
			</div>
			<div class="modal-body">
';
		/* line 115 */ $_tmp = $this->global->uiControl->getComponent("conferenceForm");
		if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
		$_tmp->render();
		echo '			</div>
			<div class="modal-footer">
				<button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Zavřít</button>
			</div>
		</div>
	</div>
</div>
';
	}


	/** {block slider} on line 64 */
	public function blockSlider(array $ʟ_args): void
	{
		
	}


	/** {block content} on line 70 */
	public function blockContent(array $ʟ_args): void
	{
		
	}

}
