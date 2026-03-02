<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Components/Conference/templates/ConferenceList.latte */
final class Templatef8d2cab1e8 extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		'snippet' => ['conferenceEditSnippet' => 'blockConferenceEditSnippet'],
	];


	public function main(): array
	{
		extract($this->params);
		echo '<div class="row my-4">
';
		if ($conferences->count() > 0) /* line 2 */ {
			$iterations = 0;
			foreach ($conferences as $conference) /* line 3 */ {
				echo '			<div class="col-sm-12 col-md-6 col-lg-4 mb-4">
				<div class="card" style="width: 100%;">
					<div class="card-image-container">
';
				if ($conference->photo) /* line 7 */ {
					echo '							<img class="card-img-top" src="';
					echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 8 */;
					echo '/';
					echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($conference->photo)) /* line 8 */;
					echo '" alt="Card image cap" style="max-height: 300px; object-fit: cover; width: 100%;">
';
				} else /* line 9 */ {
					echo '							<img class="card-img-top" src="';
					echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 10 */;
					echo '/uploads/conferences/default.webp" alt="Card image cap" style="max-height: 300px; object-fit: cover; width: 100%;">
';
				}
				if ($conference->getEndsAt() < $currentDateTime) /* line 12 */ {
					echo '							<div class="presentation-overlay">PROBĚHLO</div>
';
				}
				echo '					</div>

					<div class="card-body">
						<h5 class="card-title">';
				echo LR\Filters::escapeHtmlText($conference->title) /* line 18 */;
				echo '</h5>

						<!-- Další informace o prezentaci -->
						<div class="row pt-2">
							<div class="col-5">
								<h6 class="card-subtitle mb-2">Žánr:</h6>
							</div>
							<div class="col-7">
								<h6 class="card-subtitle mb-2">';
				echo LR\Filters::escapeHtmlText($conference->genre) /* line 26 */;
				echo '</h6>
							</div>
						</div>

						<!-- Začátek -->
						<div class="row">
							<div class="col-5">
								<h6 class="card-subtitle mb-2">Začátek:</h6>
							</div>
							<div class="col-7">
								<h6 class="card-subtitle mb-2">';
				echo LR\Filters::escapeHtmlText(($this->filters->date)($conference->getStartsAt(), 'd.m.Y H:i')) /* line 36 */;
				echo '</h6>
							</div>
						</div>

						<!-- Konec -->
						<div class="row">
							<div class="col-5">
								<h6 class="card-subtitle mb-2">Konec:</h6>
							</div>
							<div class="col-7">
								<h6 class="card-subtitle mb-2">';
				echo LR\Filters::escapeHtmlText(($this->filters->date)($conference->getEndsAt(), 'd.m.Y H:i')) /* line 46 */;
				echo '</h6>
							</div>
						</div>

						<!-- Obsazenost -->
						<div class="row">
							<div class="col-5">
								<h6 class="card-subtitle mb-2">Obsazenost:</h6>
							</div>
							<div class="col-7">
								<h6 class="card-subtitle mb-2">';
				echo LR\Filters::escapeHtmlText($conference->getNumOfAttendees()) /* line 56 */;
				echo '/';
				echo LR\Filters::escapeHtmlText($conference->capacity) /* line 56 */;
				echo '</h6>
							</div>
						</div>

						<p class="card-text">';
				echo LR\Filters::escapeHtmlText($conference->description) /* line 60 */;
				echo '</p>
						<a href="';
				echo LR\Filters::escapeHtmlAttr($this->global->uiPresenter->link(":Front:Conference:detail", ['id' => $conference->getId()])) /* line 61 */;
				echo '" class="btn btn-primary">Více informací</a>

';
				if ($user->isLoggedIn() && $conference->getUser()->getId() === $user->getId()) /* line 63 */ {
					echo '							<a class="btn btn-warning ajax" data-bs-toggle="modal" data-bs-target="#dialog-conf" href="';
					echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link("edit!", [$conference->getId()])) /* line 64 */;
					echo '">Upravit</a>
';
				}
				echo '					</div>
				</div>
			</div>
';
				$iterations++;
			}
			echo "\n";
		} else /* line 71 */ {
			echo '		<h5 class="text-center">Nenalezeny žádné konference.</h5>
';
		}
		echo '</div>


<div class="modal fade" id="dialog-conf" tabindex="-1" aria-labelledby="dialog-label" aria-hidden="true">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<h5 class="modal-title" id="dialog-label">Upravit konferenci</h5>
				<button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Zavřít">
				</button>
			</div>
			<div class="modal-body">
<div id="';
		echo htmlspecialchars($this->global->snippetDriver->getHtmlId('conferenceEditSnippet'));
		echo '">';
		$this->renderBlock('conferenceEditSnippet', [], null, 'snippet') /* line 86 */;
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
		if (!$this->getReferringTemplate() || $this->getReferenceType() === "extends") {
			foreach (array_intersect_key(['conference' => '3'], $this->params) as $ʟ_v => $ʟ_l) {
				trigger_error("Variable \$$ʟ_v overwritten in foreach on line $ʟ_l");
			}
		}
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {snippet conferenceEditSnippet} on line 86 */
	public function blockConferenceEditSnippet(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		$this->global->snippetDriver->enter("conferenceEditSnippet", 'static');
		try {
			if ($currentConferenceId) /* line 87 */ {
				/* line 88 */ $_tmp = $this->global->uiControl->getComponent("conferenceEditForm");
				if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
				$_tmp->render();
			} else /* line 89 */ {
				echo '						<div class="text-center">
							<div class="spinner-border" role="status">
							</div>
						</div>
';
			}
		} finally {
			$this->global->snippetDriver->leave();
		}
		
	}

}
