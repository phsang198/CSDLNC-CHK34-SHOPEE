import { Database } from 'arangojs';
import { ConfigService } from '@nestjs/config';

export const arangoProvider = {
  provide: 'DATABASE_CONNECTION',
  useFactory: (configService: ConfigService) => {
    const db = new Database({
      url: configService.get('ARANGO_DB_URL'),
    });
    db.userDatabases();
    db.useBasicAuth(
      configService.get('ARANGO_DB_USER'),
      configService.get('ARANGO_DB_PASSWORD'),
    );
    return db;
  },
  inject: [ConfigService],
};
