<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Components/Presentation/templates/PresentationList.latte */
final class Template2324d83fd5 extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		'snippet' => ['presentationEditSnippet' => 'blockPresentationEditSnippet'],
	];


	public function main(): array
	{
		extract($this->params);
		echo '<div class="row my-4">
';
		if ($presentations->count() > 0) /* line 2 */ {
			$iterations = 0;
			foreach ($presentations as $presentation) /* line 3 */ {
				echo '			<div class="col-sm-12 col-md-6 col-lg-4 mb-4">
				<div class="card" style="width: 100%;">
					<div class="card-image-container">
';
				if ($presentation->photo) /* line 7 */ {
					echo '							<img class="card-img-top" src="';
					echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 8 */;
					echo '/';
					echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($presentation->photo)) /* line 8 */;
					echo '" alt="Card image cap" style="max-height: 300px; object-fit: cover; width: 100%;">
';
				} else /* line 9 */ {
					echo '							<img class="card-img-top" src="';
					echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 10 */;
					echo '/uploads/presentations/default.webp" alt="Card image cap" style="max-height: 300px; object-fit: cover; width: 100%;">
';
				}
				if (isset($presentation->endsAt) && $presentation->endsAt < $currentDateTime) /* line 12 */ {
					echo '							<div class="presentation-overlay">PROBĚHLO</div>
';
				}
				echo '					</div>

                    <div class="card-body">
                        <h5 class="card-title">';
				echo LR\Filters::escapeHtmlText($presentation->title) /* line 18 */;
				echo '</h5>

                        <!-- Zvětšené štítky schválení -->
';
				if ($user->isLoggedIn() && ($presentation->speaker->getId() === $user->getId() || $presentation->conference->getUser()->getId() === $user->getId() || $user->isInRole('admin'))) /* line 21 */ {
					if ($presentation->state === 1) /* line 22 */ {
						echo '                                <span class="badge bg-warning mb-3" style="font-size: 0.9rem; padding: 10px;">Čeká na schválení</span>
';
					} elseif ($presentation->state === 2) /* line 24 */ {
						echo '                                <span class="badge bg-success mb-3" style="font-size: 0.9rem; padding: 10px;">Schváleno</span>
';
					} elseif ($presentation->state === 3) /* line 26 */ {
						echo '                                <span class="badge bg-danger mb-3" style="font-size: 0.9rem; padding: 10px;">Neschváleno</span>
';
					}
				}
				echo '
						<!-- Začátek -->
						<div class="row">
							<div class="col-5">
								<h6 class="card-subtitle mb-2">Začátek:</h6>
							</div>
							<div class="col-7">
';
				if (isset($presentation->startsAt)) /* line 37 */ {
					echo '									<h6 class="card-subtitle mb-2">';
					echo LR\Filters::escapeHtmlText(($this->filters->date)($presentation->startsAt, 'd.m.Y H:i')) /* line 38 */;
					echo '</h6>
';
				} else /* line 39 */ {
					echo '									<h6 class="card-subtitle mb-2">TBD</h6>
';
				}
				echo '							</div>
						</div>

						<!-- Konec -->
						<div class="row">
							<div class="col-5">
								<h6 class="card-subtitle mb-2">Konec:</h6>
							</div>
							<div class="col-7">
';
				if (isset($presentation->endsAt)) /* line 51 */ {
					echo '									<h6 class="card-subtitle mb-2">';
					echo LR\Filters::escapeHtmlText(($this->filters->date)($presentation->endsAt, 'd.m.Y H:i')) /* line 52 */;
					echo '</h6>
';
				} else /* line 53 */ {
					echo '									<h6 class="card-subtitle mb-2">TBD</h6>
';
				}
				echo '
							</div>
						</div>

                        <!-- Přednášející -->
                        <div class="row">
                            <div class="col-5">
                                <h6 class="card-subtitle mb-2">Přednášející:</h6>
                            </div>
                            <div class="col-7">
                                <h6 class="card-subtitle mb-2">';
				echo LR\Filters::escapeHtmlText($presentation->speaker->getFullName()) /* line 66 */;
				echo '</h6>
                            </div>
                        </div>

					<!-- Konec -->
						<div class="row">
								<div class="col-5">
									<h6 class="card-subtitle mb-2">Počet účastníků:</h6>
								</div>
								<div class="col-7">
									<h6 class="card-subtitle mb-2">';
				echo LR\Filters::escapeHtmlText($presentation->attendances->count()) /* line 76 */;
				echo '</h6>
								</div>
                        </div>

						<a href="';
				echo LR\Filters::escapeHtmlAttr($this->global->uiPresenter->link(":Front:Presentations:detail", ['id' => $presentation->getId()])) /* line 80 */;
				echo '" class="btn btn-primary">Více informací</a>
';
				if ($user->isLoggedIn() && ($presentation->speaker->getId() === $user->getId()
				|| $presentation->conference->getUser()->getId() === $user->getId())) /* line 81 */ {
					echo '							<a class="btn btn-warning ajax" data-bs-toggle="modal" data-bs-target="#dialog-';
					echo LR\Filters::escapeHtmlAttr($presentation->getId()) /* line 83 */;
					echo '" href="';
					echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link("edit!", [$presentation->getId()])) /* line 83 */;
					echo '">Upravit</a>
';
				}
				echo '					</div>
				</div>
			</div>

			<!-- Unikátní modální okno pro každou prezentaci -->
			<div class="modal fade" id="dialog-';
				echo LR\Filters::escapeHtmlAttr($presentation->getId()) /* line 90 */;
				echo '" tabindex="-1" aria-labelledby="dialog-label-';
				echo LR\Filters::escapeHtmlAttr($presentation->getId()) /* line 90 */;
				echo '" aria-hidden="true">
				<div class="modal-dialog">
					<div class="modal-content">
						<div class="modal-header">
							<h5 class="modal-title" id="dialog-label-';
				echo LR\Filters::escapeHtmlAttr($presentation->getId()) /* line 94 */;
				echo '">Upravit prezentaci</h5>
							<button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Zavřít"></button>
						</div>
						<div class="modal-body">
<div id="';
				echo htmlspecialchars($this->global->snippetDriver->getHtmlId('presentationEditSnippet'));
				echo '">';
				$this->renderBlock('presentationEditSnippet', [], null, 'snippet') /* line 98 */;
				echo '</div>
						</div>
						<div class="modal-footer">
							<button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Zavřít</button>
						</div>
					</div>
				</div>
			</div>

';
				$iterations++;
			}
			echo "\n";
		} else /* line 117 */ {
			echo '		<h5 class="text-center">Nenalezeny žádné prezentace.</h5>
';
		}
		echo '</div>
';
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		if (!$this->getReferringTemplate() || $this->getReferenceType() === "extends") {
			foreach (array_intersect_key(['presentation' => '3'], $this->params) as $ʟ_v => $ʟ_l) {
				trigger_error("Variable \$$ʟ_v overwritten in foreach on line $ʟ_l");
			}
		}
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {snippet presentationEditSnippet} on line 98 */
	public function blockPresentationEditSnippet(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		$this->global->snippetDriver->enter("presentationEditSnippet", 'static');
		try {
			if ($currentPresentationId) /* line 99 */ {
				/* line 100 */ $_tmp = $this->global->uiControl->getComponent("presentationEditForm");
				if ($_tmp instanceof Nette\Application\UI\Renderable) $_tmp->redrawControl(null, false);
				$_tmp->render();
			} else /* line 101 */ {
				echo '									<div class="text-center">
										<div class="spinner-border" role="status"></div>
									</div>
';
			}
		} finally {
			$this->global->snippetDriver->leave();
		}
		
	}

}
