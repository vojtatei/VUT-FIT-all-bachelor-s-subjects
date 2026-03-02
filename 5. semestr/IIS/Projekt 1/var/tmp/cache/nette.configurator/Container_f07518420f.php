<?php
// source: /var/www/html/app/../config/env/prod.neon
// source: /var/www/html/app/../config/local.neon
// source: array
// source: array

/** @noinspection PhpParamsInspection,PhpMethodMayBeStaticInspection */

declare(strict_types=1);

class Container_f07518420f extends Nette\DI\Container
{
	protected array $tags = [
		'nette.inject' => [
			'application.1' => true,
			'application.10' => true,
			'application.11' => true,
			'application.12' => true,
			'application.13' => true,
			'application.14' => true,
			'application.15' => true,
			'application.16' => true,
			'application.2' => true,
			'application.3' => true,
			'application.4' => true,
			'application.5' => true,
			'application.6' => true,
			'application.7' => true,
			'application.8' => true,
			'application.9' => true,
			'nettrine.fixtures.loadDataFixturesCommand' => true,
		],
		'console.command' => [
			'nettrine.fixtures.loadDataFixturesCommand' => 'doctrine:fixtures:load',
			'nettrine.migrations.currentCommand' => 'migrations:current',
			'nettrine.migrations.diffCommand' => 'migrations:diff',
			'nettrine.migrations.dumpSchemaCommand' => 'migrations:dump-schema',
			'nettrine.migrations.executeCommand' => 'migrations:execute',
			'nettrine.migrations.generateCommand' => 'migrations:generate',
			'nettrine.migrations.latestCommand' => 'migrations:latest',
			'nettrine.migrations.listCommand' => 'migrations:list',
			'nettrine.migrations.migrateCommand' => 'migrations:migrate',
			'nettrine.migrations.rollupCommand' => 'migrations:rollup',
			'nettrine.migrations.statusCommand' => 'migrations:status',
			'nettrine.migrations.syncMetadataCommand' => 'migrations:sync-metadata-storage',
			'nettrine.migrations.upToDateCommand' => 'migrations:up-to-date',
			'nettrine.migrations.versionCommand' => 'migrations:version',
		],
		'nettrine.orm.annotation.driver' => ['nettrine.orm.annotations.annotationDriver' => true],
		'nettrine.orm.mapping.driver' => ['nettrine.orm.mappingDriver' => true],
	];

	protected array $aliases = [
		'application' => 'application.application',
		'cacheStorage' => 'cache.storage',
		'httpRequest' => 'http.request',
		'httpResponse' => 'http.response',
		'nette.cacheJournal' => 'cache.journal',
		'nette.httpRequestFactory' => 'http.requestFactory',
		'nette.latteFactory' => 'latte.latteFactory',
		'nette.mailer' => 'mail.mailer',
		'nette.presenterFactory' => 'application.presenterFactory',
		'nette.templateFactory' => 'latte.templateFactory',
		'nette.userStorage' => 'security.userStorage',
		'session' => 'session.session',
		'user' => 'security.user',
	];

	protected array $wiring = [
		'Nette\DI\Container' => [['container']],
		'Nette\Application\Application' => [['application.application']],
		'Nette\Application\IPresenterFactory' => [['application.presenterFactory']],
		'Nette\Application\LinkGenerator' => [['application.linkGenerator']],
		'Nette\Caching\Storages\Journal' => [['cache.journal']],
		'Nette\Caching\Storage' => [['cache.storage']],
		'Nette\Http\RequestFactory' => [['http.requestFactory']],
		'Nette\Http\IRequest' => [['http.request']],
		'Nette\Http\Request' => [['http.request']],
		'Nette\Http\IResponse' => [['http.response']],
		'Nette\Http\Response' => [['http.response']],
		'Nette\Bridges\ApplicationLatte\LatteFactory' => [['latte.latteFactory']],
		'Nette\Application\UI\TemplateFactory' => [['latte.templateFactory']],
		'Nette\Bridges\ApplicationLatte\TemplateFactory' => [['latte.templateFactory']],
		'Nette\Mail\Mailer' => [['mail.mailer']],
		'Nette\Security\Passwords' => [['security.passwords']],
		'App\Model\Security\Passwords' => [['security.passwords']],
		'Nette\Security\UserStorage' => [['security.userStorage']],
		'Nette\Security\User' => [['security.user']],
		'App\Model\Security\SecurityUser' => [['security.user']],
		'Nette\Http\Session' => [['session.session']],
		'Tracy\ILogger' => [
			0 => ['contributte.monolog.psrToTracyLazyAdapter'],
			2 => ['tracy.logger', 'contributte.monolog.psrToTracyAdapter'],
		],
		'Tracy\BlueScreen' => [['tracy.blueScreen']],
		'Tracy\Bar' => [['tracy.bar']],
		'Symfony\Contracts\EventDispatcher\EventDispatcherInterface' => [
			0 => ['contributte.events.dispatcher.tracy'],
			2 => ['contributte.events.dispatcher'],
		],
		'Psr\EventDispatcher\EventDispatcherInterface' => [
			0 => ['contributte.events.dispatcher.tracy'],
			2 => ['contributte.events.dispatcher'],
		],
		'Symfony\Component\EventDispatcher\EventDispatcherInterface' => [
			0 => ['contributte.events.dispatcher.tracy'],
			2 => ['contributte.events.dispatcher'],
		],
		'Monolog\Handler\StreamHandler' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\Handler\AbstractProcessingHandler' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\Handler\AbstractHandler' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\Handler\Handler' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\Handler\HandlerInterface' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\ResettableInterface' => [
			0 => ['contributte.monolog.logger.default'],
			2 => ['contributte.monolog.logger.default.handler.0'],
		],
		'Monolog\Handler\ProcessableHandlerInterface' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\Handler\FormattableHandlerInterface' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\Handler\RotatingFileHandler' => [2 => ['contributte.monolog.logger.default.handler.0']],
		'Monolog\Processor\ProcessorInterface' => [
			2 => [
				'contributte.monolog.logger.default.processor.0',
				'contributte.monolog.logger.default.processor.1',
				'contributte.monolog.logger.default.processor.2',
				'contributte.monolog.logger.default.processor.3',
			],
		],
		'Monolog\Processor\WebProcessor' => [2 => ['contributte.monolog.logger.default.processor.0']],
		'Monolog\Processor\IntrospectionProcessor' => [2 => ['contributte.monolog.logger.default.processor.1']],
		'Monolog\Processor\MemoryProcessor' => [2 => ['contributte.monolog.logger.default.processor.2']],
		'Monolog\Processor\MemoryPeakUsageProcessor' => [2 => ['contributte.monolog.logger.default.processor.2']],
		'Monolog\Processor\ProcessIdProcessor' => [2 => ['contributte.monolog.logger.default.processor.3']],
		'Psr\Log\LoggerInterface' => [['contributte.monolog.logger.default']],
		'Monolog\Logger' => [['contributte.monolog.logger.default']],
		'Tracy\Bridges\Psr\PsrToTracyLoggerAdapter' => [2 => ['contributte.monolog.psrToTracyAdapter']],
		'Contributte\Monolog\Tracy\LazyTracyLogger' => [['contributte.monolog.psrToTracyLazyAdapter']],
		'Contributte\Mailing\IMailBuilderFactory' => [['contributte.mailing.builderFactory']],
		'Contributte\Mailing\IMailSender' => [['contributte.mailing.sender']],
		'Contributte\Mailing\IMailTemplateFactory' => [['contributte.mailing.templateFactory']],
		'Contributte\Mail\Message\IMessageFactory' => [['contributte.post.messageFactory']],
		'Doctrine\Common\Annotations\Reader' => [
			0 => ['nettrine.annotations.reader'],
			2 => ['nettrine.annotations.delegatedReader'],
		],
		'Doctrine\Common\Annotations\AnnotationReader' => [2 => ['nettrine.annotations.delegatedReader']],
		'Doctrine\Common\Cache\Cache' => [['nettrine.cache.driver']],
		'Doctrine\Migrations\Metadata\Storage\MetadataStorageConfiguration' => [
			['nettrine.migrations.configuration.tableStorage'],
		],
		'Doctrine\Migrations\Metadata\Storage\TableMetadataStorageConfiguration' => [
			['nettrine.migrations.configuration.tableStorage'],
		],
		'Doctrine\Migrations\Configuration\Configuration' => [['nettrine.migrations.configuration']],
		'Doctrine\Migrations\Version\MigrationFactory' => [['nettrine.migrations.migrationFactory']],
		'Nettrine\Migrations\Version\DbalMigrationFactory' => [['nettrine.migrations.migrationFactory']],
		'Nettrine\Migrations\DI\DependencyFactory' => [2 => ['nettrine.migrations.nettrineDependencyFactory']],
		'Doctrine\Migrations\DependencyFactory' => [['nettrine.migrations.dependencyFactory']],
		'Doctrine\Migrations\Tools\Console\Command\DoctrineCommand' => [
			2 => [
				'nettrine.migrations.currentCommand',
				'nettrine.migrations.diffCommand',
				'nettrine.migrations.dumpSchemaCommand',
				'nettrine.migrations.executeCommand',
				'nettrine.migrations.generateCommand',
				'nettrine.migrations.latestCommand',
				'nettrine.migrations.listCommand',
				'nettrine.migrations.migrateCommand',
				'nettrine.migrations.rollupCommand',
				'nettrine.migrations.statusCommand',
				'nettrine.migrations.syncMetadataCommand',
				'nettrine.migrations.upToDateCommand',
				'nettrine.migrations.versionCommand',
			],
		],
		'Symfony\Component\Console\Command\Command' => [
			0 => ['nettrine.fixtures.loadDataFixturesCommand'],
			2 => [
				'nettrine.migrations.currentCommand',
				'nettrine.migrations.diffCommand',
				'nettrine.migrations.dumpSchemaCommand',
				'nettrine.migrations.executeCommand',
				'nettrine.migrations.generateCommand',
				'nettrine.migrations.latestCommand',
				'nettrine.migrations.listCommand',
				'nettrine.migrations.migrateCommand',
				'nettrine.migrations.rollupCommand',
				'nettrine.migrations.statusCommand',
				'nettrine.migrations.syncMetadataCommand',
				'nettrine.migrations.upToDateCommand',
				'nettrine.migrations.versionCommand',
			],
		],
		'Doctrine\Migrations\Tools\Console\Command\CurrentCommand' => [2 => ['nettrine.migrations.currentCommand']],
		'Doctrine\Migrations\Tools\Console\Command\DiffCommand' => [2 => ['nettrine.migrations.diffCommand']],
		'Doctrine\Migrations\Tools\Console\Command\DumpSchemaCommand' => [2 => ['nettrine.migrations.dumpSchemaCommand']],
		'Doctrine\Migrations\Tools\Console\Command\ExecuteCommand' => [2 => ['nettrine.migrations.executeCommand']],
		'Doctrine\Migrations\Tools\Console\Command\GenerateCommand' => [2 => ['nettrine.migrations.generateCommand']],
		'Doctrine\Migrations\Tools\Console\Command\LatestCommand' => [2 => ['nettrine.migrations.latestCommand']],
		'Doctrine\Migrations\Tools\Console\Command\ListCommand' => [2 => ['nettrine.migrations.listCommand']],
		'Doctrine\Migrations\Tools\Console\Command\MigrateCommand' => [2 => ['nettrine.migrations.migrateCommand']],
		'Doctrine\Migrations\Tools\Console\Command\RollupCommand' => [2 => ['nettrine.migrations.rollupCommand']],
		'Doctrine\Migrations\Tools\Console\Command\StatusCommand' => [2 => ['nettrine.migrations.statusCommand']],
		'Doctrine\Migrations\Tools\Console\Command\SyncMetadataCommand' => [
			2 => ['nettrine.migrations.syncMetadataCommand'],
		],
		'Doctrine\Migrations\Tools\Console\Command\UpToDateCommand' => [2 => ['nettrine.migrations.upToDateCommand']],
		'Doctrine\Migrations\Tools\Console\Command\VersionCommand' => [2 => ['nettrine.migrations.versionCommand']],
		'Doctrine\Common\DataFixtures\Loader' => [['nettrine.fixtures.fixturesLoader']],
		'Nettrine\Fixtures\Loader\FixturesLoader' => [['nettrine.fixtures.fixturesLoader']],
		'Nettrine\Fixtures\Command\LoadDataFixturesCommand' => [['nettrine.fixtures.loadDataFixturesCommand']],
		'Doctrine\DBAL\Logging\SQLLogger' => [
			['nettrine.dbal.profiler'],
			['nettrine.dbal.logger'],
			['nettrine.dbal.logger.config'],
		],
		'Nettrine\DBAL\Logger\PsrLogger' => [2 => ['nettrine.dbal.logger.config']],
		'Nettrine\DBAL\Logger\AbstractLogger' => [['nettrine.dbal.profiler']],
		'Nettrine\DBAL\Logger\ProfilerLogger' => [['nettrine.dbal.profiler']],
		'Doctrine\DBAL\Logging\LoggerChain' => [['nettrine.dbal.logger']],
		'Doctrine\DBAL\Configuration' => [0 => ['nettrine.orm.configuration'], 2 => ['nettrine.dbal.configuration']],
		'Doctrine\Common\EventManager' => [0 => ['nettrine.dbal.eventManager.debug'], 2 => ['nettrine.dbal.eventManager']],
		'Nettrine\DBAL\Events\ContainerAwareEventManager' => [2 => ['nettrine.dbal.eventManager']],
		'Nettrine\DBAL\Events\DebugEventManager' => [['nettrine.dbal.eventManager.debug']],
		'Nettrine\DBAL\ConnectionFactory' => [['nettrine.dbal.connectionFactory']],
		'Doctrine\DBAL\Connection' => [['nettrine.dbal.connection']],
		'Nettrine\DBAL\ConnectionAccessor' => [['nettrine.dbal.connectionAccessor']],
		'Doctrine\ORM\Configuration' => [['nettrine.orm.configuration']],
		'Doctrine\ORM\Mapping\EntityListenerResolver' => [['nettrine.orm.entityListenerResolver']],
		'Nettrine\ORM\Mapping\ContainerEntityListenerResolver' => [['nettrine.orm.entityListenerResolver']],
		'Doctrine\ORM\Decorator\EntityManagerDecorator' => [['nettrine.orm.entityManagerDecorator']],
		'Doctrine\Persistence\ObjectManagerDecorator' => [['nettrine.orm.entityManagerDecorator']],
		'Doctrine\ORM\EntityManagerInterface' => [['nettrine.orm.entityManagerDecorator']],
		'Doctrine\Persistence\ObjectManager' => [['nettrine.orm.entityManagerDecorator']],
		'App\Model\Database\EntityManagerDecorator' => [['nettrine.orm.entityManagerDecorator']],
		'Doctrine\Persistence\AbstractManagerRegistry' => [['nettrine.orm.managerRegistry']],
		'Doctrine\Persistence\ConnectionRegistry' => [['nettrine.orm.managerRegistry']],
		'Doctrine\Persistence\ManagerRegistry' => [['nettrine.orm.managerRegistry']],
		'Nettrine\ORM\ManagerRegistry' => [['nettrine.orm.managerRegistry']],
		'Doctrine\Persistence\Mapping\Driver\MappingDriver' => [
			0 => ['nettrine.orm.mappingDriver'],
			2 => [1 => 'nettrine.orm.annotations.annotationDriver'],
		],
		'Doctrine\Persistence\Mapping\Driver\MappingDriverChain' => [['nettrine.orm.mappingDriver']],
		'Doctrine\ORM\Cache\RegionsConfiguration' => [2 => ['nettrine.orm.cache.regions']],
		'Doctrine\ORM\Cache\CacheFactory' => [2 => ['nettrine.orm.cache.cacheFactory']],
		'Doctrine\ORM\Cache\DefaultCacheFactory' => [2 => ['nettrine.orm.cache.cacheFactory']],
		'Doctrine\ORM\Cache\CacheConfiguration' => [2 => ['nettrine.orm.cache.cacheConfiguration']],
		'Doctrine\ORM\Mapping\Driver\CompatibilityAnnotationDriver' => [2 => ['nettrine.orm.annotations.annotationDriver']],
		'Doctrine\ORM\Mapping\Driver\AnnotationDriver' => [2 => ['nettrine.orm.annotations.annotationDriver']],
		'Nette\Security\Authenticator' => [['security.authenticator']],
		'Nette\Security\IdentityHandler' => [['security.authenticator']],
		'Nette\Security\IAuthenticator' => [['security.authenticator']],
		'App\Model\Security\Authenticator\UserAuthenticator' => [['security.authenticator']],
		'Nette\Security\Permission' => [['security.authorizator']],
		'Nette\Security\Authorizator' => [['security.authorizator']],
		'App\Model\Security\Authorizator\StaticAuthorizator' => [['security.authorizator']],
		'App\Model\Router\RouterFactory' => [['01']],
		'Nette\Routing\Router' => [['router']],
		'App\Model\Database\QueryManager' => [['02']],
		'Nette\Application\Response' => [['03']],
		'Stringable' => [['03']],
		'Contributte\PdfResponse\PdfResponse' => [['03']],
		'App\UI\Modules\Admin\BaseAdminPresenter' => [
			2 => ['application.1', 'application.2', 'application.3', 'application.4', 'application.5', 'application.6'],
		],
		'App\UI\Modules\Base\SecuredPresenter' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.8',
				'application.9',
			],
		],
		'App\UI\Modules\Base\BasePresenter' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\Application\UI\Presenter' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\Application\UI\Control' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\Application\UI\Component' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\ComponentModel\Container' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\ComponentModel\Component' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\ComponentModel\IComponent' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\ComponentModel\IContainer' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\Application\UI\SignalReceiver' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\Application\UI\StatePersistent' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'ArrayAccess' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\Application\UI\Renderable' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'Nette\Application\IPresenter' => [
			2 => [
				'application.1',
				'application.2',
				'application.3',
				'application.4',
				'application.5',
				'application.6',
				'application.7',
				'application.8',
				'application.9',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
				'application.15',
				'application.16',
			],
		],
		'App\UI\Modules\Admin\Conference\ConferencePresenter' => [2 => ['application.1']],
		'App\UI\Modules\Admin\Home\HomePresenter' => [2 => ['application.2']],
		'App\UI\Modules\Admin\Presentation\PresentationPresenter' => [2 => ['application.3']],
		'App\UI\Modules\Admin\Reservation\ReservationPresenter' => [2 => ['application.4']],
		'App\UI\Modules\Admin\Room\RoomPresenter' => [2 => ['application.5']],
		'App\UI\Modules\Admin\User\UserPresenter' => [2 => ['application.6']],
		'App\UI\Modules\Front\BaseFrontPresenter' => [
			2 => [
				'application.7',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'App\UI\Modules\Base\UnsecuredPresenter' => [
			2 => [
				'application.7',
				'application.10',
				'application.11',
				'application.12',
				'application.13',
				'application.14',
			],
		],
		'App\UI\Modules\Front\Conference\ConferencePresenter' => [2 => ['application.7']],
		'App\UI\Modules\Base\BaseErrorPresenter' => [2 => ['application.8']],
		'App\UI\Modules\Front\Error\ErrorPresenter' => [2 => ['application.8']],
		'App\UI\Modules\Base\BaseError4xxPresenter' => [2 => ['application.9']],
		'App\UI\Modules\Front\Error4xx\Error4xxPresenter' => [2 => ['application.9']],
		'App\UI\Modules\Front\Home\HomePresenter' => [2 => ['application.10']],
		'App\UI\Modules\Front\Presentations\PresentationsPresenter' => [2 => ['application.11']],
		'App\UI\Modules\Front\Reservations\ReservationsPresenter' => [2 => ['application.12']],
		'App\UI\Modules\Front\Sign\SignPresenter' => [2 => ['application.13']],
		'App\UI\Modules\Front\User\UserPresenter' => [2 => ['application.14']],
		'NetteModule\ErrorPresenter' => [2 => ['application.15']],
		'NetteModule\MicroPresenter' => [2 => ['application.16']],
		'App\Model\Services\ICrudService' => [['04', '06', '07', '08', '09', '010']],
		'App\Model\Services\AttendanceService' => [['04']],
		'App\Model\Services\ConferenceService' => [['05']],
		'App\Model\Services\PresentationService' => [['06']],
		'App\Model\Services\QuestionService' => [['07']],
		'App\Model\Services\ReservationService' => [['08']],
		'App\Model\Services\RoomService' => [['09']],
		'App\Model\Services\UserService' => [['010']],
		'App\UI\Components\Conference\ConferenceFormFactory' => [['011']],
		'App\UI\Components\Conference\ConferenceGridFactory' => [['012']],
		'App\UI\Components\Conference\ConferenceListFactory' => [['013']],
		'App\UI\Components\Presentation\PresentationFormFactory' => [['014']],
		'App\UI\Components\Presentation\PresentationGridFactory' => [['015']],
		'App\UI\Components\Presentation\PresentationListFactory' => [['016']],
		'App\UI\Components\Presentation\ScheduleListFactory' => [['017']],
		'App\UI\Components\Question\QuestionFormFactory' => [['018']],
		'App\UI\Components\Question\QuestionGridFactory' => [['019']],
		'App\UI\Components\Reservation\MyReservationsGridFactory' => [['020']],
		'App\UI\Components\Reservation\ReservationFormFactory' => [['021']],
		'App\UI\Components\Reservation\ReservationGridFactory' => [['022']],
		'App\UI\Components\Room\RoomFormFactory' => [['023']],
		'App\UI\Components\Room\RoomGridFactory' => [['024']],
		'App\UI\Components\Sign\SignUpFormFactory' => [['025']],
		'App\UI\Components\User\UserGridFactory' => [['026']],
		'App\UI\Form\FormFactory' => [['027']],
	];


	public function __construct(array $params = [])
	{
		parent::__construct($params);
	}


	public function createService01(): App\Model\Router\RouterFactory
	{
		return new App\Model\Router\RouterFactory;
	}


	public function createService02(): App\Model\Database\QueryManager
	{
		return new App\Model\Database\QueryManager($this->getService('nettrine.orm.entityManagerDecorator'));
	}


	public function createService03(): Contributte\PdfResponse\PdfResponse
	{
		$service = new Contributte\PdfResponse\PdfResponse;
		$service->mpdfConfig = ['tempDir' => '/var/www/html/app/../var/tmp/mpdf'];
		return $service;
	}


	public function createService04(): App\Model\Services\AttendanceService
	{
		return new App\Model\Services\AttendanceService($this->getService('nettrine.orm.entityManagerDecorator'));
	}


	public function createService05(): App\Model\Services\ConferenceService
	{
		return new App\Model\Services\ConferenceService(
			$this->getService('nettrine.orm.entityManagerDecorator'),
			$this->getService('security.user'),
		);
	}


	public function createService06(): App\Model\Services\PresentationService
	{
		return new App\Model\Services\PresentationService($this->getService('nettrine.orm.entityManagerDecorator'));
	}


	public function createService07(): App\Model\Services\QuestionService
	{
		return new App\Model\Services\QuestionService($this->getService('nettrine.orm.entityManagerDecorator'));
	}


	public function createService08(): App\Model\Services\ReservationService
	{
		return new App\Model\Services\ReservationService($this->getService('nettrine.orm.entityManagerDecorator'));
	}


	public function createService09(): App\Model\Services\RoomService
	{
		return new App\Model\Services\RoomService($this->getService('nettrine.orm.entityManagerDecorator'));
	}


	public function createService010(): App\Model\Services\UserService
	{
		return new App\Model\Services\UserService($this->getService('nettrine.orm.entityManagerDecorator'));
	}


	public function createService011(): App\UI\Components\Conference\ConferenceFormFactory
	{
		return new class ($this) implements App\UI\Components\Conference\ConferenceFormFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(): App\UI\Components\Conference\ConferenceForm
			{
				return new App\UI\Components\Conference\ConferenceForm($this->container->getService('027'), $this->container->getService('05'));
			}
		};
	}


	public function createService012(): App\UI\Components\Conference\ConferenceGridFactory
	{
		return new class ($this) implements App\UI\Components\Conference\ConferenceGridFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(): App\UI\Components\Conference\ConferenceGrid
			{
				return new App\UI\Components\Conference\ConferenceGrid($this->container->getService('05'));
			}
		};
	}


	public function createService013(): App\UI\Components\Conference\ConferenceListFactory
	{
		return new class ($this) implements App\UI\Components\Conference\ConferenceListFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(
				Doctrine\Common\Collections\ArrayCollection $conferences,
			): App\UI\Components\Conference\ConferenceList
			{
				return new App\UI\Components\Conference\ConferenceList($this->container->getService('05'), $conferences, $this->container->getService('011'));
			}
		};
	}


	public function createService014(): App\UI\Components\Presentation\PresentationFormFactory
	{
		return new class ($this) implements App\UI\Components\Presentation\PresentationFormFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(
				int $conferenceId,
				?App\Domain\Presentation\Presentation $presentation = null,
			): App\UI\Components\Presentation\PresentationForm
			{
				return new App\UI\Components\Presentation\PresentationForm(
					$this->container->getService('06'),
					$this->container->getService('027'),
					$conferenceId,
					$presentation,
				);
			}
		};
	}


	public function createService015(): App\UI\Components\Presentation\PresentationGridFactory
	{
		return new class ($this) implements App\UI\Components\Presentation\PresentationGridFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(): App\UI\Components\Presentation\PresentationGrid
			{
				return new App\UI\Components\Presentation\PresentationGrid($this->container->getService('06'), $this->container->getService('014'));
			}
		};
	}


	public function createService016(): App\UI\Components\Presentation\PresentationListFactory
	{
		return new class ($this) implements App\UI\Components\Presentation\PresentationListFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(
				Doctrine\Common\Collections\ArrayCollection $presentations,
			): App\UI\Components\Presentation\PresentationList
			{
				return new App\UI\Components\Presentation\PresentationList($this->container->getService('06'), $presentations, $this->container->getService('014'));
			}
		};
	}


	public function createService017(): App\UI\Components\Presentation\ScheduleListFactory
	{
		return new class ($this) implements App\UI\Components\Presentation\ScheduleListFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(
				Doctrine\Common\Collections\ArrayCollection $presentations = new \Doctrine\Common\Collections\ArrayCollection(/* unknown */),
			): App\UI\Components\Presentation\ScheduleList
			{
				return new App\UI\Components\Presentation\ScheduleList($this->container->getService('06'), $this->container->getService('04'), $presentations);
			}
		};
	}


	public function createService018(): App\UI\Components\Question\QuestionFormFactory
	{
		return new class ($this) implements App\UI\Components\Question\QuestionFormFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(
				int $presentationId,
				?int $userId,
				?App\Domain\Question\Question $question = null,
			): App\UI\Components\Question\QuestionForm
			{
				return new App\UI\Components\Question\QuestionForm(
					$this->container->getService('027'),
					$this->container->getService('07'),
					$presentationId,
					$userId,
					$question,
				);
			}
		};
	}


	public function createService019(): App\UI\Components\Question\QuestionGridFactory
	{
		return new class ($this) implements App\UI\Components\Question\QuestionGridFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(?int $presentationId = null): App\UI\Components\Question\QuestionGrid
			{
				return new App\UI\Components\Question\QuestionGrid($this->container->getService('07'), $this->container->getService('018'), $presentationId);
			}
		};
	}


	public function createService020(): App\UI\Components\Reservation\MyReservationsGridFactory
	{
		return new class ($this) implements App\UI\Components\Reservation\MyReservationsGridFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(): App\UI\Components\Reservation\MyReservationsGrid
			{
				return new App\UI\Components\Reservation\MyReservationsGrid(
					$this->container->getService('08'),
					$this->container->getService('06'),
					$this->container->getService('017'),
				);
			}
		};
	}


	public function createService021(): App\UI\Components\Reservation\ReservationFormFactory
	{
		return new class ($this) implements App\UI\Components\Reservation\ReservationFormFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(int $conferenceId, ?int $userId = null): App\UI\Components\Reservation\ReservationForm
			{
				return new App\UI\Components\Reservation\ReservationForm(
					$this->container->getService('027'),
					$this->container->getService('08'),
					$this->container->getService('010'),
					$this->container->getService('05'),
					$conferenceId,
					$userId,
				);
			}
		};
	}


	public function createService022(): App\UI\Components\Reservation\ReservationGridFactory
	{
		return new class ($this) implements App\UI\Components\Reservation\ReservationGridFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(?int $conferenceId = null): App\UI\Components\Reservation\ReservationGrid
			{
				return new App\UI\Components\Reservation\ReservationGrid($this->container->getService('08'), $conferenceId);
			}
		};
	}


	public function createService023(): App\UI\Components\Room\RoomFormFactory
	{
		return new class ($this) implements App\UI\Components\Room\RoomFormFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(int $conferenceId, ?App\Domain\Room\Room $room = null): App\UI\Components\Room\RoomForm
			{
				return new App\UI\Components\Room\RoomForm($this->container->getService('027'), $this->container->getService('09'), $conferenceId, $room);
			}
		};
	}


	public function createService024(): App\UI\Components\Room\RoomGridFactory
	{
		return new class ($this) implements App\UI\Components\Room\RoomGridFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(int $conferenceId): App\UI\Components\Room\RoomGrid
			{
				return new App\UI\Components\Room\RoomGrid($this->container->getService('09'), $this->container->getService('023'), $conferenceId);
			}
		};
	}


	public function createService025(): App\UI\Components\Sign\SignUpFormFactory
	{
		return new class ($this) implements App\UI\Components\Sign\SignUpFormFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(?App\Domain\User\User $user = null): App\UI\Components\Sign\SignUpForm
			{
				return new App\UI\Components\Sign\SignUpForm($this->container->getService('010'), $this->container->getService('027'), $user);
			}
		};
	}


	public function createService026(): App\UI\Components\User\UserGridFactory
	{
		return new class ($this) implements App\UI\Components\User\UserGridFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(): App\UI\Components\User\UserGrid
			{
				return new App\UI\Components\User\UserGrid($this->container->getService('010'));
			}
		};
	}


	public function createService027(): App\UI\Form\FormFactory
	{
		return new App\UI\Form\FormFactory($this->getService('security.user'));
	}


	public function createServiceApplication__1(): App\UI\Modules\Admin\Conference\ConferencePresenter
	{
		$service = new App\UI\Modules\Admin\Conference\ConferencePresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->conferenceService = $this->getService('05');
		$service->conferenceGridFactory = $this->getService('012');
		$service->conferenceFormFactory = $this->getService('011');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__10(): App\UI\Modules\Front\Home\HomePresenter
	{
		$service = new App\UI\Modules\Front\Home\HomePresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->presentationService = $this->getService('06');
		$service->presentationListFactory = $this->getService('016');
		$service->conferenceService = $this->getService('05');
		$service->conferenceListFactory = $this->getService('013');
		$service->conferenceFormFactory = $this->getService('011');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__11(): App\UI\Modules\Front\Presentations\PresentationsPresenter
	{
		$service = new App\UI\Modules\Front\Presentations\PresentationsPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->questionGridFactory = $this->getService('019');
		$service->questionFormFactory = $this->getService('018');
		$service->presentationService = $this->getService('06');
		$service->presentationListFactory = $this->getService('016');
		$service->presentationFormFactory = $this->getService('014');
		$service->conferenceFormFactory = $this->getService('011');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__12(): App\UI\Modules\Front\Reservations\ReservationsPresenter
	{
		$service = new App\UI\Modules\Front\Reservations\ReservationsPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->gridFactory = $this->getService('020');
		$service->conferenceFormFactory = $this->getService('011');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__13(): App\UI\Modules\Front\Sign\SignPresenter
	{
		$service = new App\UI\Modules\Front\Sign\SignPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->userService = $this->getService('010');
		$service->signUpFormFactory = $this->getService('025');
		$service->formFactory = $this->getService('027');
		$service->conferenceFormFactory = $this->getService('011');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__14(): App\UI\Modules\Front\User\UserPresenter
	{
		$service = new App\UI\Modules\Front\User\UserPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->injectRequireLoggedUser();
		$service->userService = $this->getService('010');
		$service->signUpFormFactory = $this->getService('025');
		$service->presentationService = $this->getService('06');
		$service->presentationListFactory = $this->getService('016');
		$service->formFactory = $this->getService('027');
		$service->conferenceService = $this->getService('05');
		$service->conferenceListFactory = $this->getService('013');
		$service->conferenceFormFactory = $this->getService('011');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__15(): NetteModule\ErrorPresenter
	{
		return new NetteModule\ErrorPresenter($this->getService('contributte.monolog.psrToTracyLazyAdapter'));
	}


	public function createServiceApplication__16(): NetteModule\MicroPresenter
	{
		return new NetteModule\MicroPresenter($this, $this->getService('http.request'), $this->getService('router'));
	}


	public function createServiceApplication__2(): App\UI\Modules\Admin\Home\HomePresenter
	{
		$service = new App\UI\Modules\Admin\Home\HomePresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__3(): App\UI\Modules\Admin\Presentation\PresentationPresenter
	{
		$service = new App\UI\Modules\Admin\Presentation\PresentationPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->gridFactory = $this->getService('015');
		$service->conferenceService = $this->getService('05');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__4(): App\UI\Modules\Admin\Reservation\ReservationPresenter
	{
		$service = new App\UI\Modules\Admin\Reservation\ReservationPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->gridFactory = $this->getService('022');
		$service->conferenceService = $this->getService('05');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__5(): App\UI\Modules\Admin\Room\RoomPresenter
	{
		$service = new App\UI\Modules\Admin\Room\RoomPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->roomService = $this->getService('09');
		$service->roomGridFactory = $this->getService('024');
		$service->roomFormFactory = $this->getService('023');
		$service->conferenceService = $this->getService('05');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__6(): App\UI\Modules\Admin\User\UserPresenter
	{
		$service = new App\UI\Modules\Admin\User\UserPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->userGridFactory = $this->getService('026');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__7(): App\UI\Modules\Front\Conference\ConferencePresenter
	{
		$service = new App\UI\Modules\Front\Conference\ConferencePresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->scheduleListFactory = $this->getService('017');
		$service->roomGridFactory = $this->getService('024');
		$service->roomFormFactory = $this->getService('023');
		$service->reservationGridFactory = $this->getService('022');
		$service->reservationFormFactory = $this->getService('021');
		$service->presentationService = $this->getService('06');
		$service->presentationListFactory = $this->getService('016');
		$service->presentationFormFactory = $this->getService('014');
		$service->conferenceService = $this->getService('05');
		$service->conferenceListFactory = $this->getService('013');
		$service->conferenceFormFactory = $this->getService('011');
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__8(): App\UI\Modules\Front\Error\ErrorPresenter
	{
		$service = new App\UI\Modules\Front\Error\ErrorPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__9(): App\UI\Modules\Front\Error4xx\Error4xxPresenter
	{
		$service = new App\UI\Modules\Front\Error4xx\Error4xxPresenter;
		$service->injectPrimary(
			$this->getService('http.request'),
			$this->getService('http.response'),
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('session.session'),
			$this->getService('security.user'),
			$this->getService('latte.templateFactory'),
		);
		$service->invalidLinkMode = 5;
		return $service;
	}


	public function createServiceApplication__application(): Nette\Application\Application
	{
		$service = new Nette\Application\Application(
			$this->getService('application.presenterFactory'),
			$this->getService('router'),
			$this->getService('http.request'),
			$this->getService('http.response'),
		);
		Nette\Bridges\ApplicationDI\ApplicationExtension::initializeBlueScreenPanel(
			$this->getService('tracy.blueScreen'),
			$service,
		);
		$this->getService('tracy.bar')->addPanel(new Nette\Bridges\ApplicationTracy\RoutingPanel(
			$this->getService('router'),
			$this->getService('http.request'),
			$this->getService('application.presenterFactory'),
		));
		$service->onStartup[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\StartupEvent(...func_get_args()));};
		$service->onError[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\ErrorEvent(...func_get_args()));};
		$service->onPresenter[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\PresenterEvent(...func_get_args()));};
		$service->onPresenter[] = function($application, $presenter) {if(!$presenter instanceof Nette\Application\UI\Presenter){return;} $presenter->onStartup[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\PresenterStartupEvent(...func_get_args()));};};
		$service->onPresenter[] = function($application, $presenter) {if(!$presenter instanceof Nette\Application\UI\Presenter){return;} $presenter->onShutdown[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\PresenterShutdownEvent(...func_get_args()));};};
		$service->onRequest[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\RequestEvent(...func_get_args()));};
		$service->onResponse[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\ResponseEvent(...func_get_args()));};
		$service->onShutdown[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Application\ShutdownEvent(...func_get_args()));};
		return $service;
	}


	public function createServiceApplication__linkGenerator(): Nette\Application\LinkGenerator
	{
		return new Nette\Application\LinkGenerator(
			$this->getService('router'),
			$this->getService('http.request')->getUrl()->withoutUserInfo(),
			$this->getService('application.presenterFactory'),
		);
	}


	public function createServiceApplication__presenterFactory(): Nette\Application\IPresenterFactory
	{
		$service = new Nette\Application\PresenterFactory(new Nette\Bridges\ApplicationDI\PresenterFactoryCallback(
			$this,
			5,
			'/var/www/html/app/../var/tmp/cache/nette.application/touch',
		));
		$service->setMapping([
			'Admin' => ['App\UI\Modules\Admin', '*', '*\*Presenter'],
			'Front' => ['App\UI\Modules\Front', '*', '*\*Presenter'],
		]);
		return $service;
	}


	public function createServiceCache__journal(): Nette\Caching\Storages\Journal
	{
		return new Nette\Caching\Storages\SQLiteJournal('/var/www/html/app/../var/tmp/cache/journal.s3db');
	}


	public function createServiceCache__storage(): Nette\Caching\Storage
	{
		return new Nette\Caching\Storages\FileStorage('/var/www/html/app/../var/tmp/cache', $this->getService('cache.journal'));
	}


	public function createServiceContainer(): Nette\DI\Container
	{
		return $this;
	}


	public function createServiceContributte__events__dispatcher(
	): Symfony\Component\EventDispatcher\EventDispatcherInterface
	{
		return new Symfony\Component\EventDispatcher\EventDispatcher;
	}


	public function createServiceContributte__events__dispatcher__tracy(
	): Symfony\Component\EventDispatcher\EventDispatcherInterface
	{
		return new Contributte\EventDispatcher\Diagnostics\TracyDispatcher($this->getService('contributte.events.dispatcher'));
	}


	public function createServiceContributte__mailing__builderFactory(): Contributte\Mailing\IMailBuilderFactory
	{
		return new Contributte\Mailing\MailBuilderFactory(
			$this->getService('contributte.mailing.sender'),
			$this->getService('contributte.mailing.templateFactory'),
		);
	}


	public function createServiceContributte__mailing__sender(): Contributte\Mailing\IMailSender
	{
		return new Contributte\Mailing\NetteMailSender($this->getService('mail.mailer'));
	}


	public function createServiceContributte__mailing__templateFactory(): Contributte\Mailing\IMailTemplateFactory
	{
		$service = new Contributte\Mailing\NetteTemplateFactory(
			$this->getService('latte.templateFactory'),
			$this->getService('application.linkGenerator'),
		);
		$service->setDefaults(['layout' => '@default']);
		$service->setConfig(['layout' => '/var/www/html/app/resources/mail/@layout.latte']);
		return $service;
	}


	public function createServiceContributte__monolog__logger__default(): Monolog\Logger
	{
		return new Monolog\Logger(
			'default',
			[$this->getService('contributte.monolog.logger.default.handler.0')],
			[
			$this->getService('contributte.monolog.logger.default.processor.0'),
			$this->getService('contributte.monolog.logger.default.processor.1'),
			$this->getService('contributte.monolog.logger.default.processor.2'),
			$this->getService('contributte.monolog.logger.default.processor.3'),
		],
		);
	}


	public function createServiceContributte__monolog__logger__default__handler__0(): Monolog\Handler\RotatingFileHandler
	{
		return new Monolog\Handler\RotatingFileHandler('/var/www/html/app/../var/log/syslog.log', 30, 300);
	}


	public function createServiceContributte__monolog__logger__default__processor__0(): Monolog\Processor\WebProcessor
	{
		return new Monolog\Processor\WebProcessor;
	}


	public function createServiceContributte__monolog__logger__default__processor__1(
	): Monolog\Processor\IntrospectionProcessor
	{
		return new Monolog\Processor\IntrospectionProcessor;
	}


	public function createServiceContributte__monolog__logger__default__processor__2(
	): Monolog\Processor\MemoryPeakUsageProcessor
	{
		return new Monolog\Processor\MemoryPeakUsageProcessor;
	}


	public function createServiceContributte__monolog__logger__default__processor__3(
	): Monolog\Processor\ProcessIdProcessor
	{
		return new Monolog\Processor\ProcessIdProcessor;
	}


	public function createServiceContributte__monolog__psrToTracyAdapter(): Tracy\Bridges\Psr\PsrToTracyLoggerAdapter
	{
		return new Tracy\Bridges\Psr\PsrToTracyLoggerAdapter($this->getService('contributte.monolog.logger.default'));
	}


	public function createServiceContributte__monolog__psrToTracyLazyAdapter(): Contributte\Monolog\Tracy\LazyTracyLogger
	{
		return new Contributte\Monolog\Tracy\LazyTracyLogger('contributte.monolog.psrToTracyAdapter', $this);
	}


	public function createServiceContributte__post__messageFactory(): Contributte\Mail\Message\IMessageFactory
	{
		return new class ($this) implements Contributte\Mail\Message\IMessageFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(): Nette\Mail\Message
			{
				return new Nette\Mail\Message;
			}
		};
	}


	public function createServiceHttp__request(): Nette\Http\Request
	{
		return $this->getService('http.requestFactory')->fromGlobals();
	}


	public function createServiceHttp__requestFactory(): Nette\Http\RequestFactory
	{
		$service = new Nette\Http\RequestFactory;
		$service->setProxy([]);
		return $service;
	}


	public function createServiceHttp__response(): Nette\Http\Response
	{
		$service = new Nette\Http\Response;
		$service->cookieSecure = $this->getService('http.request')->isSecured();
		return $service;
	}


	public function createServiceLatte__latteFactory(): Nette\Bridges\ApplicationLatte\LatteFactory
	{
		return new class ($this) implements Nette\Bridges\ApplicationLatte\LatteFactory {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function create(): Latte\Engine
			{
				$service = new Latte\Engine;
				$service->setTempDirectory('/var/www/html/app/../var/tmp/cache/latte');
				$service->setAutoRefresh(true);
				$service->setStrictTypes(false);
				$service->onCompile[] = function() {$this->container->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Latte\LatteCompileEvent(...func_get_args()));};
				return $service;
			}
		};
	}


	public function createServiceLatte__templateFactory(): Nette\Bridges\ApplicationLatte\TemplateFactory
	{
		$service = new Nette\Bridges\ApplicationLatte\TemplateFactory(
			$this->getService('latte.latteFactory'),
			$this->getService('http.request'),
			$this->getService('security.user'),
			$this->getService('cache.storage'),
			null,
		);
		Nette\Bridges\ApplicationDI\LatteExtension::initLattePanel($service, $this->getService('tracy.bar'), false);
		$service->onCreate[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Latte\TemplateCreateEvent(...func_get_args()));};
		return $service;
	}


	public function createServiceMail__mailer(): Nette\Mail\Mailer
	{
		return new Nette\Mail\SendmailMailer;
	}


	public function createServiceNettrine__annotations__delegatedReader(): Doctrine\Common\Annotations\AnnotationReader
	{
		$service = new Doctrine\Common\Annotations\AnnotationReader;
		$service->addGlobalIgnoredName('persistent');
		$service->addGlobalIgnoredName('serializationVersion');
		return $service;
	}


	public function createServiceNettrine__annotations__reader(): Doctrine\Common\Annotations\Reader
	{
		return new Doctrine\Common\Annotations\CachedReader(
			$this->getService('nettrine.annotations.delegatedReader'),
			$this->getService('nettrine.cache.driver'),
			false,
		);
	}


	public function createServiceNettrine__cache__driver(): Doctrine\Common\Cache\Cache
	{
		return new Doctrine\Common\Cache\PhpFileCache('/var/www/html/app/../var/tmp/cache/nettrine.cache');
	}


	public function createServiceNettrine__dbal__configuration(): Doctrine\DBAL\Configuration
	{
		$service = new Doctrine\DBAL\Configuration;
		$service->setSQLLogger($this->getService('nettrine.dbal.logger'));
		$service->setResultCacheImpl($this->getService('nettrine.cache.driver'));
		$service->setAutoCommit(true);
		return $service;
	}


	public function createServiceNettrine__dbal__connection(): Doctrine\DBAL\Connection
	{
		$service = $this->getService('nettrine.dbal.connectionFactory')->createConnection(
			[
			'driver' => 'mysqli',
			'host' => 'web-conference-db',
			'user' => 'contributte',
			'password' => 'contributte',
			'dbname' => 'contributte',
			'port' => 3306,
			'charset' => 'utf8',
			'default_table_options' => ['charset' => 'utf8', 'collate' => 'utf8_unicode_ci'],
			'types' => [],
			'typesMapping' => [],
		],
			$this->getService('nettrine.dbal.configuration'),
			$this->getService('nettrine.dbal.eventManager.debug'),
		);
		$this->getService('tracy.bar')->addPanel(new Nettrine\DBAL\Tracy\QueryPanel\QueryPanel($this->getService('nettrine.dbal.profiler')));
		$this->getService('tracy.blueScreen')->addPanel(['Nettrine\DBAL\Tracy\BlueScreen\DbalBlueScreen', 'renderException']);
		return $service;
	}


	public function createServiceNettrine__dbal__connectionAccessor(): Nettrine\DBAL\ConnectionAccessor
	{
		return new class ($this) implements Nettrine\DBAL\ConnectionAccessor {
			public function __construct(
				private Container_f07518420f $container,
			) {
			}


			public function get(): Doctrine\DBAL\Connection
			{
				return $this->container->getService('nettrine.dbal.connection');
			}
		};
	}


	public function createServiceNettrine__dbal__connectionFactory(): Nettrine\DBAL\ConnectionFactory
	{
		return new Nettrine\DBAL\ConnectionFactory([], []);
	}


	public function createServiceNettrine__dbal__eventManager(): Nettrine\DBAL\Events\ContainerAwareEventManager
	{
		return new Nettrine\DBAL\Events\ContainerAwareEventManager($this);
	}


	public function createServiceNettrine__dbal__eventManager__debug(): Nettrine\DBAL\Events\DebugEventManager
	{
		return new Nettrine\DBAL\Events\DebugEventManager($this->getService('nettrine.dbal.eventManager'));
	}


	public function createServiceNettrine__dbal__logger(): Doctrine\DBAL\Logging\LoggerChain
	{
		return new Doctrine\DBAL\Logging\LoggerChain([
			$this->getService('nettrine.dbal.logger.config'),
			$this->getService('nettrine.dbal.profiler'),
		]);
	}


	public function createServiceNettrine__dbal__logger__config(): Nettrine\DBAL\Logger\PsrLogger
	{
		return new Nettrine\DBAL\Logger\PsrLogger($this->getService('contributte.monolog.logger.default'));
	}


	public function createServiceNettrine__dbal__profiler(): Nettrine\DBAL\Logger\ProfilerLogger
	{
		return new Nettrine\DBAL\Logger\ProfilerLogger($this->getService('nettrine.dbal.connectionAccessor'));
	}


	public function createServiceNettrine__fixtures__fixturesLoader(): Nettrine\Fixtures\Loader\FixturesLoader
	{
		return new Nettrine\Fixtures\Loader\FixturesLoader(['/var/www/html/db/Fixtures'], $this);
	}


	public function createServiceNettrine__fixtures__loadDataFixturesCommand(
	): Nettrine\Fixtures\Command\LoadDataFixturesCommand
	{
		return new Nettrine\Fixtures\Command\LoadDataFixturesCommand(
			$this->getService('nettrine.fixtures.fixturesLoader'),
			$this->getService('nettrine.orm.managerRegistry'),
		);
	}


	public function createServiceNettrine__migrations__configuration(): Doctrine\Migrations\Configuration\Configuration
	{
		$service = new Doctrine\Migrations\Configuration\Configuration;
		$service->setCustomTemplate(null);
		$service->setMetadataStorageConfiguration($this->getService('nettrine.migrations.configuration.tableStorage'));
		$service->addMigrationsDirectory('Database\Migrations', '/var/www/html/db/Migrations');
		$service->setAllOrNothing(false);
		return $service;
	}


	public function createServiceNettrine__migrations__configuration__tableStorage(
	): Doctrine\Migrations\Metadata\Storage\TableMetadataStorageConfiguration
	{
		$service = new Doctrine\Migrations\Metadata\Storage\TableMetadataStorageConfiguration;
		$service->setTableName('doctrine_migrations');
		$service->setVersionColumnName('version');
		return $service;
	}


	public function createServiceNettrine__migrations__currentCommand(
	): Doctrine\Migrations\Tools\Console\Command\CurrentCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\CurrentCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__dependencyFactory(): Doctrine\Migrations\DependencyFactory
	{
		return $this->getService('nettrine.migrations.nettrineDependencyFactory')->createDependencyFactory();
	}


	public function createServiceNettrine__migrations__diffCommand(
	): Doctrine\Migrations\Tools\Console\Command\DiffCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\DiffCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__dumpSchemaCommand(
	): Doctrine\Migrations\Tools\Console\Command\DumpSchemaCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\DumpSchemaCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__executeCommand(
	): Doctrine\Migrations\Tools\Console\Command\ExecuteCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\ExecuteCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__generateCommand(
	): Doctrine\Migrations\Tools\Console\Command\GenerateCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\GenerateCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__latestCommand(
	): Doctrine\Migrations\Tools\Console\Command\LatestCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\LatestCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__listCommand(
	): Doctrine\Migrations\Tools\Console\Command\ListCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\ListCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__migrateCommand(
	): Doctrine\Migrations\Tools\Console\Command\MigrateCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\MigrateCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__migrationFactory(
	): Nettrine\Migrations\Version\DbalMigrationFactory
	{
		return new Nettrine\Migrations\Version\DbalMigrationFactory(
			$this,
			$this->getService('nettrine.dbal.connection'),
			$this->getService('contributte.monolog.logger.default'),
		);
	}


	public function createServiceNettrine__migrations__nettrineDependencyFactory(
	): Nettrine\Migrations\DI\DependencyFactory
	{
		return new Nettrine\Migrations\DI\DependencyFactory(
			$this->getService('nettrine.migrations.configuration'),
			$this->getService('nettrine.migrations.migrationFactory'),
			$this->getService('nettrine.dbal.connection'),
			$this->getService('nettrine.orm.entityManagerDecorator'),
			$this->getService('contributte.monolog.logger.default'),
		);
	}


	public function createServiceNettrine__migrations__rollupCommand(
	): Doctrine\Migrations\Tools\Console\Command\RollupCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\RollupCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__statusCommand(
	): Doctrine\Migrations\Tools\Console\Command\StatusCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\StatusCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__syncMetadataCommand(
	): Doctrine\Migrations\Tools\Console\Command\SyncMetadataCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\SyncMetadataCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__upToDateCommand(
	): Doctrine\Migrations\Tools\Console\Command\UpToDateCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\UpToDateCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__migrations__versionCommand(
	): Doctrine\Migrations\Tools\Console\Command\VersionCommand
	{
		return new Doctrine\Migrations\Tools\Console\Command\VersionCommand($this->getService('nettrine.migrations.dependencyFactory'));
	}


	public function createServiceNettrine__orm__annotations__annotationDriver(
	): Doctrine\ORM\Mapping\Driver\AnnotationDriver
	{
		$service = new Doctrine\ORM\Mapping\Driver\AnnotationDriver($this->getService('nettrine.annotations.reader'));
		$service->addExcludePaths([]);
		$service->addPaths(['/var/www/html/app/Domain']);
		return $service;
	}


	public function createServiceNettrine__orm__cache__cacheConfiguration(): Doctrine\ORM\Cache\CacheConfiguration
	{
		$service = new Doctrine\ORM\Cache\CacheConfiguration;
		$service->setCacheFactory($this->getService('nettrine.orm.cache.cacheFactory'));
		return $service;
	}


	public function createServiceNettrine__orm__cache__cacheFactory(): Doctrine\ORM\Cache\DefaultCacheFactory
	{
		return new Doctrine\ORM\Cache\DefaultCacheFactory(
			$this->getService('nettrine.orm.cache.regions'),
			$this->getService('nettrine.cache.driver'),
		);
	}


	public function createServiceNettrine__orm__cache__regions(): Doctrine\ORM\Cache\RegionsConfiguration
	{
		return new Doctrine\ORM\Cache\RegionsConfiguration;
	}


	public function createServiceNettrine__orm__configuration(): Doctrine\ORM\Configuration
	{
		$service = new Doctrine\ORM\Configuration;
		$service->setProxyDir('/var/www/html/app/../var/tmp/proxies');
		$service->setAutoGenerateProxyClasses(2);
		$service->setProxyNamespace('Nettrine\Proxy');
		$service->setMetadataDriverImpl($this->getService('nettrine.orm.mappingDriver'));
		$service->setCustomStringFunctions([]);
		$service->setCustomNumericFunctions([]);
		$service->setCustomDatetimeFunctions([]);
		$service->setCustomHydrationModes([]);
		$service->setNamingStrategy(new Doctrine\ORM\Mapping\UnderscoreNamingStrategy);
		$service->setEntityListenerResolver($this->getService('nettrine.orm.entityListenerResolver'));
		$service->setQueryCacheImpl($this->getService('nettrine.cache.driver'));
		$service->setHydrationCacheImpl($this->getService('nettrine.cache.driver'));
		$service->setResultCacheImpl($this->getService('nettrine.cache.driver'));
		$service->setMetadataCacheImpl($this->getService('nettrine.cache.driver'));
		$service->setSecondLevelCacheEnabled(true);
		$service->setSecondLevelCacheConfiguration($this->getService('nettrine.orm.cache.cacheConfiguration'));
		return $service;
	}


	public function createServiceNettrine__orm__entityListenerResolver(
	): Nettrine\ORM\Mapping\ContainerEntityListenerResolver
	{
		return new Nettrine\ORM\Mapping\ContainerEntityListenerResolver($this);
	}


	public function createServiceNettrine__orm__entityManagerDecorator(): App\Model\Database\EntityManagerDecorator
	{
		return new App\Model\Database\EntityManagerDecorator(Doctrine\ORM\EntityManager::create(
			$this->getService('nettrine.dbal.connection'),
			$this->getService('nettrine.orm.configuration'),
			$this->getService('nettrine.dbal.eventManager.debug'),
		));
	}


	public function createServiceNettrine__orm__managerRegistry(): Nettrine\ORM\ManagerRegistry
	{
		return new Nettrine\ORM\ManagerRegistry(
			$this->getService('nettrine.dbal.connection'),
			$this->getService('nettrine.orm.entityManagerDecorator'),
			$this,
		);
	}


	public function createServiceNettrine__orm__mappingDriver(): Doctrine\Persistence\Mapping\Driver\MappingDriverChain
	{
		$service = new Doctrine\Persistence\Mapping\Driver\MappingDriverChain;
		$service->addDriver($this->getService('nettrine.orm.annotations.annotationDriver'), 'App\Domain');
		return $service;
	}


	public function createServiceRouter(): Nette\Routing\Router
	{
		return $this->getService('01')->create();
	}


	public function createServiceSecurity__authenticator(): App\Model\Security\Authenticator\UserAuthenticator
	{
		return new App\Model\Security\Authenticator\UserAuthenticator(
			$this->getService('02'),
			$this->getService('nettrine.orm.entityManagerDecorator'),
			$this->getService('security.passwords'),
		);
	}


	public function createServiceSecurity__authorizator(): App\Model\Security\Authorizator\StaticAuthorizator
	{
		return new App\Model\Security\Authorizator\StaticAuthorizator;
	}


	public function createServiceSecurity__passwords(): App\Model\Security\Passwords
	{
		return new App\Model\Security\Passwords;
	}


	public function createServiceSecurity__user(): App\Model\Security\SecurityUser
	{
		$service = new App\Model\Security\SecurityUser(
			$this->getService('security.userStorage'),
			$this->getService('security.authenticator'),
			$this->getService('security.authorizator'),
		);
		$this->getService('tracy.bar')->addPanel(new Nette\Bridges\SecurityTracy\UserPanel($service));
		$service->onLoggedIn[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Security\LoggedInEvent(...func_get_args()));};
		$service->onLoggedOut[] = function() {$this->getService('contributte.events.dispatcher.tracy')->dispatch(new Contributte\Events\Extra\Event\Security\LoggedOutEvent(...func_get_args()));};
		return $service;
	}


	public function createServiceSecurity__userStorage(): Nette\Security\UserStorage
	{
		$service = new Nette\Bridges\SecurityHttp\SessionStorage($this->getService('session.session'));
		$service->setNamespace('Webapp');
		return $service;
	}


	public function createServiceSession__session(): Nette\Http\Session
	{
		$service = new Nette\Http\Session($this->getService('http.request'), $this->getService('http.response'));
		$service->setExpiration('1 year');
		$service->setOptions([
			'cookieHttponly' => true,
			'cookieSamesite' => 'Lax',
			'name' => 'SID',
			'sidBitsPerCharacter' => 6,
			'sidLength' => 128,
			'useCookies' => true,
			'useOnlyCookies' => true,
			'useStrictMode' => true,
		]);
		return $service;
	}


	public function createServiceTracy__bar(): Tracy\Bar
	{
		return Tracy\Debugger::getBar();
	}


	public function createServiceTracy__blueScreen(): Tracy\BlueScreen
	{
		return Tracy\Debugger::getBlueScreen();
	}


	public function createServiceTracy__logger(): Tracy\ILogger
	{
		return Tracy\Debugger::getLogger();
	}


	public function initialize(): void
	{
		Doctrine\Common\Annotations\AnnotationRegistry::registerUniqueLoader("class_exists");
		// di.
		(function () {
			$this->getService('tracy.bar')->addPanel(new Nette\Bridges\DITracy\ContainerPanel($this));
		})();
		// http.
		(function () {
			$response = $this->getService('http.response');
			$response->setHeader('X-Powered-By', 'Nette Framework 3');
			$response->setHeader('Content-Type', 'text/html; charset=utf-8');
			$response->setHeader('X-Frame-Options', 'SAMEORIGIN');
			Nette\Http\Helpers::initCookie($this->getService('http.request'), $response);
		})();
		// php.
		(function () {
			ini_set('date.timezone', (string) ('Europe/Prague'));
			ini_set('output_buffering', (string) (4096));
		})();
		// session.
		(function () {
			$this->getService('session.session')->autoStart(false);
		})();
		// tracy.
		(function () {
			if (!Tracy\Debugger::isEnabled()) { return; }
			$logger = $this->getService('tracy.logger');
			Tracy\Debugger::$email = 'dev@dev.dev';
			Tracy\Debugger::$logSeverity = 32767;
			Tracy\Debugger::$strictMode = true;
			if ($logger instanceof Tracy\Logger) $logger->mailer = [
				new Tracy\Bridges\Nette\MailSender(
					$this->getService('mail.mailer'),
					null,
					$this->getByType('Nette\Http\Request', false)?->getUrl()->getHost(),
				),
				'send',
			];
		})();
		// contributte.events.
		(function () {
			$this->getService('tracy.bar')->addPanel(new Contributte\EventDispatcher\Tracy\EventPanel($this->getService('contributte.events.dispatcher.tracy')));
		})();
		$this->getService("tracy.logger");
		Tracy\Debugger::setLogger($this->getService('contributte.monolog.psrToTracyLazyAdapter'));
		Contributte\Monolog\LoggerHolder::setLogger('contributte.monolog.logger.default', $this);
	}


	protected function getStaticParameters(): array
	{
		return [
			'system' => ['error' => ['email' => 'dev@dev.dev', 'presenter' => 'Front:Error']],
			'database' => [
				'driver' => 'mysqli',
				'port' => 3306,
				'host' => 'web-conference-db',
				'dbname' => 'contributte',
				'user' => 'contributte',
				'password' => 'contributte',
			],
			'project' => ['rev' => '1.0.0'],
			'files' => ['root' => '/var/www/html/app/../data/files'],
			'mailing' => ['from' => 'webapp@localhost', 'from_name' => 'Webapp'],
			'smtp' => ['host' => 'localhost', 'username' => 'fake', 'password' => 'fake'],
			'appDir' => '/var/www/html/app',
			'wwwDir' => '/var/www/html/www',
			'debugMode' => true,
			'productionMode' => false,
			'consoleMode' => false,
			'tempDir' => '/var/www/html/app/../var/tmp',
			'rootDir' => '/var/www/html',
			'scope' => 'web',
		];
	}
}
