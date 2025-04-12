import { Injectable, OnModuleInit, OnModuleDestroy } from '@nestjs/common';
import { Database } from 'arangojs';

@Injectable()
export class ArangoDBService implements OnModuleInit, OnModuleDestroy {
  private db: Database;

  async onModuleInit() {
    this.db = new Database({
      url: process.env.ARANGO_DB_URL,
    });
    this.db.useBasicAuth(
      process.env.ARANGO_DB_USER,
      process.env.ARANGO_DB_PASSWORD,
    );
  }

  async onModuleDestroy() {
    await this.db.close();
  }

  getDatabase(): Database {
    return this.db;
  }
}
