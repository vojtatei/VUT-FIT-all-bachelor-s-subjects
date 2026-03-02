<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Components/Presentation/templates/PresentationGrid.latte */
final class Template9acaefdb77 extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		'snippet' => ['presentationEditSnippet' => 'blockPresentationEditSnippet'],
	];


	public function main(): array
	{
		extract($this->params);
		/* line 1 */ $_tmp = $this->global->uiControl->getComponent("grid");
		if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
		$_tmp->render();
		echo '
<div class="modal fade" id="dialog-presentation" tabindex="-1" aria-labelledby="dialog-label" aria-hidden="true">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<h5 class="modal-title" id="dialog-label">Upravit prezentaci</h5>
				<button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Zavřít"></button>
			</div>
			<div class="modal-body">
<div id="';
		echo htmlspecialchars($this->global->snippetDriver->getHtmlId('presentationEditSnippet'));
		echo '">';
		$this->renderBlock('presentationEditSnippet', [], null, 'snippet') /* line 11 */;
		echo '</div>
			</div>
			<div class="modal-footer">
				<button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Zavřít</button>
			</div>
		</div>
	</div>
</div>
';
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {snippet presentationEditSnippet} on line 11 */
	public function blockPresentationEditSnippet(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		$this->global->snippetDriver->enter("presentationEditSnippet", 'static');
		try {
			if ($currentPresentationId) /* line 12 */ {
				/* line 13 */ $_tmp = $this->global->uiControl->getComponent("presentationEditForm");
				if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
				$_tmp->render();
			} else /* line 14 */ {
				echo '						<div class="text-center">
							<div class="spinner-border" role="status"></div>
						</div>
';
			}
		} finally {
			$this->global->snippetDriver->leave();
		}
		
	}

}
